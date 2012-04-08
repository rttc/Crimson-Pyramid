/*
 * Crimson Pyramid SQL Agent
 * Copyright (C) 2012 Matt Harris
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/param.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>
#include <sys/syslog.h>
#include "dnsagent.h"

struct dbstat
{
	unsigned int created;
	unsigned int destroyed;
};

#define HOSTNAMELEN			128

static int db_init(void);
static int db_deinit(void);
static inline char *fmtdt(const char *style, size_t len);
static void run_updates(int sid);
static int db_getsrvid(void);
static void sig_end(int s);
static void sig_hup(int s);

static MYSQL *DBhandle;
extern char *optarg;
extern int optind;
static struct dbstat *stats;
static int my_sid;
static Config *my_conf;
static unsigned short termsigs;

unsigned short verbosity;
unsigned int options;

int main(int argc, char **argv)
{
	time_t lastrun;
	char go;

	termsigs = 0;
	verbosity = 1;
	options = 0;
	my_conf = (Config *)NULL;
	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, sig_end);
	signal(SIGTERM, sig_end);
	signal(SIGQUIT, sig_end);
	signal(SIGHUP, sig_hup);

	stats = (struct dbstat *)malloc(sizeof(struct dbstat));
	if (!stats)
	{
		if (dlvl(1)) { fprintf(stdout, "malloc(stats): %s\n", strerror(errno)); }
		return(-1);
	}
	memset(stats, 0, sizeof(struct dbstat));
	stats->created = 0;
	stats->destroyed = 0;

	while ((go = getopt(argc, argv, "qvlhVc:")) >= 0)
	{
		switch (go)
		{
			case 'V':
			{
				fprintf(stdout, "Crimson Pyramid SQL Agent version %lu\n", VERSION);
				fflush(stdout);
				return(0);
				break;
			}
			case 'v':
			{
				if (verbosity < 5) { verbosity++; }
				break;
			}
			case 'q':
			{
				if (verbosity > 0) { verbosity--; }
				break;
			}
			case 'd':
			{
				options &= OPTION_DAEMON;
				break;
			}
			case 'c':
			{
				if (!optarg)
				{
					fprintf(stdout, "%s -c requires an argument.\n", argv[0]);
					return(-1);
				}
				my_conf = read_config(optarg);
				if (!my_conf)
				{
					fprintf(stdout, "%s -c %s: failed reading config file.\n", argv[0], optarg);
					return(-1);
				}
				if (my_conf && dlvl(2)) { fprintf(stdout, "Loaded configuration file from: %s\n", optarg); }
				break;
			}
			case 'h':
			default:
			{
				fprintf(stdout, "Crimson Pyramid dnsagent %lu\n", VERSION);
				fprintf(stdout, "\tusage: %s [-v|q] [-h] [-l] [-c <config file path>]\n", argv[0]);
				fflush(stdout);
				return(0);
				break;
			}
		}
	}

#ifdef CONFIG_FILE
	if (!my_conf) { my_conf = read_config(CONFIG_FILE); if (my_conf && dlvl(2)) { fprintf(stdout, "Loaded configuration file from: %s\n", CONFIG_FILE); } }
#endif
	if (!my_conf) { my_conf = read_config("dnsagent.ini"); if (my_conf && dlvl(2)) { fprintf(stdout, "Loaded configuration file from: ./dnsagent.ini\n"); } }
	if (!my_conf) { my_conf = read_config("/etc/dnsagent.ini"); if (my_conf && dlvl(2)) { fprintf(stdout, "Loaded configuration file from: /etc/dnsagent.ini\n"); } }

	if (!my_conf)
	{
		if (dlvl(1)) { fprintf(stdout, "Could not load configuration file.\n"); }
		return(1);
	}

	if (my_conf->svcdaemon)
	{
		options &= OPTION_DAEMON;
	}

	if (!db_init())
	{
		if (dlvl(1) && DBhandle) { fprintf(stdout, "Database connection failed: %s\n", mysql_error(DBhandle)); }
		else if (dlvl(1)) { fprintf(stdout, "Database connection failed: UNKNOWN\n"); }

		DBhandle = (MYSQL *)NULL;
		free_config(my_conf);
		return(1);
	}

	my_sid = db_getsrvid();
	if (my_sid < 0)
	{
		/* if (dlvl(4)) { fprintf(stdout, "Error getting server ID.\n"); } */
		free_config(my_conf);
		return(0);
	}

	if (rundaemon)
	{
		openlog("CP_dnsagent", LOG_NDELAY, LOG_DAEMON);
		run_updates(my_sid);
		lastrun = time(NULL);

		while (!(options & OPTION_TERMINATE))
		{
			if ((lastrun + my_conf->waittime) <= time(NULL))
			{
				run_updates(my_sid);
				lastrun = time(NULL);
			}
			usleep(25000);	/* 1/4 of a second at a time */
		}
	}
	else
	{
		run_updates(my_sid);
	}

	if (DBhandle) { db_deinit(); }

	if (dlvl(3))
	{
		if (rundaemon)
		{
			syslog(LOG_INFO, "Created: %u", stats->created);
			syslog(LOG_INFO, "Dropped: %u", stats->destroyed);
		}
		else
		{
			fprintf(stdout, "Created: %u\n", stats->created);
			fprintf(stdout, "Dropped: %u\n", stats->destroyed);
		}
	}

	if (rundaemon) { closelog(); }
	free_config(my_conf);
	if (stats) { free(stats); }

	return(0);
}

void sig_hup(int s)
{
	/* Needs a way to keep track of which file I ended up loading my config from before I can re-read it on sighup
	 * free_config(my_conf);
	 * read_config(); */
	if (rundaemon) { syslog(LOG_INFO, "Received SIGHUP, reloading."); }
	if (!DBhandle)
	{
		return;
	}
	db_deinit();
	if (db_init())
	{
		if (dlvl(1) && DBhandle)
		{
			if (rundaemon) { syslog(LOG_ALERT, "Database connection failed: %s", mysql_error(DBhandle)); }
			else { fprintf(stdout, "Database connection failed: %s\n", mysql_error(DBhandle)); }
		}
		else if (dlvl(1))
		{
			if (rundaemon) { syslog(LOG_ALERT, "Database connection failed: UNKNOWN"); }
			else { fprintf(stdout, "Database connection failed: UNKNOWN\n"); }
		}

		DBhandle = (MYSQL *)NULL;
		free_config(my_conf);
		exit(0);
	}
}

void sig_end(int s)
{
	if (rundaemon) { syslog(LOG_NOTICE, "Received SIG%i, shutting down.", s); }
	options &= OPTION_TERMINATE;
	termsigs++;
	if (termsigs >= 3)
	{
		exit(2);
	}
}

inline char *fmtdt(const char *style, size_t len)
{
	char *dstr;
	const time_t *utime;

	dstr = (char *)malloc(len);
	if (!dstr) { return((char *)NULL); }
	memset(dstr, 0, len);
	utime = (const time_t *)time((time_t)NULL);
	strftime(dstr, len, style, localtime((const time_t *)&utime));
	return(dstr);
}

int db_init()
{
	char tru = 1;

	if (DBhandle) { return(0); }

	DBhandle = mysql_init((MYSQL *)NULL);
	if (!DBhandle)
	{
		return(0);
	}

	if (my_conf->dbhost[0] != 0)
	{
#ifdef CPDA_SSL
		mysql_set_ssl(DBhandle, (char *)NULL, (char *)NULL, my_conf->dbcapath, (char *)NULL, "-ALL:!ADH:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA:DHE-RSA-CAMELLIA128-SHA:DHE-DSS-CAMELLIA128-SHA");
#endif

		if (dlvl(4) && rundaemon) { syslog(LOG_INFO, "CONNECT: %s@%s", my_conf->dbuser, my_conf->dbhost); }
		else if (dlvl(4)) { fprintf(stdout, "CONNECT: %s@%s\n", my_conf->dbuser, my_conf->dbhost); }
		if (!mysql_real_connect(DBhandle, my_conf->dbhost, my_conf->dbuser, my_conf->dbpass, my_conf->dbname, my_conf->dbport, NULL, 0))
		{
			return(0);
		}
	}
	else
	{
		if (dlvl(4) && rundaemon) { syslog(LOG_INFO, "CONNECT: %s@%s", my_conf->dbuser, my_conf->dbfile); }
		else if (dlvl(4)) { fprintf(stdout, "CONNECT: %s@%s\n", my_conf->dbuser, my_conf->dbfile); }

		if (!mysql_real_connect(DBhandle, NULL, my_conf->dbuser, my_conf->dbpass, my_conf->dbname, 0, my_conf->dbfile, 0))
		{
			return(0);
		}
	}

	mysql_options(DBhandle, MYSQL_OPT_RECONNECT, &tru);

	return(1);
}

int db_deinit()
{
	if (!DBhandle) { return(0); }

	mysql_close(DBhandle);
	DBhandle = (MYSQL *)NULL;

	return(1);
}

void run_updates(int sid)
{
	return;
}

int db_getsrvid()
{
	char query[QUERYLEN];
	char *hn;
	MYSQL_RES *res;
	MYSQL_ROW data;
	int sid;

	if (!DBhandle) { return(-1); }

	hn = (char *)malloc(HOSTNAMELEN);
	if (!hn)
	{
		if (dlvl(1) && rundaemon) { syslog(LOG_WARNING, "malloc(hn): %s", strerror(errno)); }
		else if (dlvl(1)) { fprintf(stdout, "malloc(hn): %s\n", strerror(errno)); }
		return(-1);
	}
	memset(hn, 0, HOSTNAMELEN);
	gethostname(hn, HOSTNAMELEN);
	if ((hn[0] == '\0') || strlen(hn) < 6)
	{
		if (dlvl(1) && rundaemon) { syslog(LOG_WARNING, "Invalid system hostname: %s", hn); }
		else if (dlvl(1)) { fprintf(stdout, "Invalid system hostname: %s\n", hn); }
		return(-1);
	}

	memset(query, 0, QUERYLEN);
	snprintf(query, QUERYLEN, "select srvid from unix_srv where servername=\"%s\"", hn);
	if (dlvl(5) && !rundaemon) { fprintf(stdout, "QUERY: %s\n", query); }

	if (mysql_query(DBhandle, query) != 0)
	{
		if (dlvl(1) && rundaemon) { syslog(LOG_WARNING, "Error finding myself in the database: %s", mysql_error(DBhandle)); }
		else if (dlvl(1)) { fprintf(stdout, "Error finding myself in the database: %s\n", mysql_error(DBhandle)); }
		return(-1);
	}
	res = mysql_store_result(DBhandle);
	if (!res)
	{
		if (dlvl(1) && rundaemon) { syslog(LOG_WARNING, "Error finding myself in the database(2): %s", mysql_error(DBhandle)); }
		else if (dlvl(1)) { fprintf(stdout, "Error finding myself in the database(2): %s\n", mysql_error(DBhandle)); }
		return(-1);
	}
	if (mysql_num_rows(res) < 1)
	{
		if (dlvl(1) && rundaemon) { syslog(LOG_WARNING, "Error finding myself in the database(3): %s is not listed.", hn); }
		else if (dlvl(1)) { fprintf(stdout, "Error finding myself in the database(3): %s is not listed.\n", hn); }
		mysql_free_result(res);
		return(-1);
	}
	data = mysql_fetch_row(res);

	sid = stringtoint(data[0]);

	mysql_free_result(res);
	if (dlvl(5) && rundaemon) { syslog(LOG_INFO, "Found myself in the unix_srv database: %s=%i", hn, sid); }
	else if (dlvl(5)) { fprintf(stdout, "Found myself in the unix_srv database: %s=%i\n", hn, sid); }
	free(hn);
	return(sid);
}
