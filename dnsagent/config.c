/*
 * Crimson Pyramid DNS Agent
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
#include <limits.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include "inilib/ini.h"
#include "dnsagent.h"

static int hdlr_config(void *pc, const char *s, const char *n, const char *v);
static short fexist(const char *fname);

Config *read_config(const char *filename)
{
	Config *active_conf;
	struct stat *sb;

	active_conf = (Config *)malloc(sizeof(Config));
	if (!active_conf)
	{
		if (dlvl(1)) { fprintf(stdout, "config.c: malloc(active_conf): %s\n", strerror(errno)); }
	}
	memset(active_conf, 0, sizeof(Config));

	active_conf->dbport = 3306;
	active_conf->waittime = 60;
	sprintf(active_conf->dbname, "cp");
	sprintf(active_conf->dbfile, "/var/lib/mysql/mysql.sock");
	sprintf(active_conf->dbcapath, "/usr/local/etc/ca.crt");

	if (ini_parse(filename, hdlr_config, (void *)active_conf) != 0)
	{
		fprintf(stdout, "Failed loading %s: %s\n", filename, strerror(errno));
		free(active_conf);
		return((Config *)NULL);
	}

	if ((active_conf->dbhost[0] == 0) && (active_conf->dbfile[0] == 0))
	{
		fprintf(stdout, "Failed loading %s: neither dbhost nor dbfile set\n", filename);
		free(active_conf);
		return((Config *)NULL);
	}
	if ((active_conf->dbhost[0] == 0) && (active_conf->dbfile[0] != 0))
	{
		sb = (struct stat *)malloc(sizeof(struct stat));
		if (!sb) { return((Config *)NULL); }
		if (stat(active_conf->dbfile, sb) != 0)
		{
			if (dlvl(1)) { fprintf(stdout, "Socket error: %s: %s\n", active_conf->dbfile, strerror(errno)); }
			return((Config *)NULL);
		}
		if (!(sb->st_mode & S_IFSOCK))
		{
			if (dlvl(1)) { fprintf(stdout, "Socket error: %s: not a valid socket\n", active_conf->dbfile); }
			return((Config *)NULL);
		}
	}
	if (active_conf->dbcapath[0] != 0)
	{
		/* Always warn, but only a failure condition when using a remote mysql server */
		if (!fexist(active_conf->dbcapath))
		{
			if (dlvl(1)) { fprintf(stdout, "File error: %s is not accessible: %s\n", active_conf->dbcapath, strerror(errno)); }
		}
	}

	return(active_conf);
}

void free_config(Config *c)
{
	if (!c) { return; }

	/* we destroy the contents of the memory containing our database password before releasing it */
	memset(c, 0, sizeof(Config));
	free(c);
}

int hdlr_config(void *pc, const char *s, const char *n, const char *v)
{
	Config *c;

	if (!pc) { return(0); }
	c = (Config *)pc;

/*
        char dbname[128];
        char dbhost[128];
        int dbport;
        char dbuser[64];
        char dbpass[QUERYLEN];
        char dbfile[512];
        char dbcapath[512];

        short svcdaemon;
        char svcsrvid[16];
*/
	if ((strcasecmp(s, "database") == 0) && (strcasecmp(n, "host") == 0))
	{
		if (!v || (strlen(v) < 1) || (strcasecmp(v, "localhost") == 0) || (strcasecmp(v, "127.0.0.1") == 0))
		{
			memset(c->dbhost, 0, 128);
		}
		else
		{
			memset(c->dbhost, 0, 128);
			strncpy(c->dbhost, v, 128);
		}
	}
	else if ((strcasecmp(s, "database") == 0) && (strcasecmp(n, "name") == 0))
	{
		if (!v)
		{
			return(0);
		}
		strncpy(c->dbname, v, 128);
	}
	else if ((strcasecmp(s, "database") == 0) && (strcasecmp(n, "user") == 0))
	{
		if (!v)
		{
			return(0);
		}
		strncpy(c->dbuser, v, 64);
	}
	else if ((strcasecmp(s, "database") == 0) && (strcasecmp(n, "pass") == 0))
	{
		if (!v)
		{
			return(0);
		}
		strncpy(c->dbpass, v, QUERYLEN);
	}
	else if ((strcasecmp(s, "database") == 0) && (strcasecmp(n, "file") == 0))
	{
		if (!v)
		{
			memset(c->dbfile, 0, 512);
			return(0);
		}
		strncpy(c->dbfile, v, 512);
	}
	else if ((strcasecmp(s, "database") == 0) && (strcasecmp(n, "capath") == 0))
	{
		if (!v)
		{
			memset(c->dbcapath, 0, 512);
			return(0);
		}
		strncpy(c->dbcapath, v, 512);
	}
	else if ((strcasecmp(s, "database") == 0) && (strcasecmp(n, "port") == 0))
	{
		if (!v)
		{
			return(0);
		}
		c->dbport = stringtouint(v);
	}
	else if ((strcasecmp(s, "server") == 0) && (strcasecmp(n, "daemon") == 0))
	{
		if (!v)
		{
			return(0);
		}
		if (strcasecmp(v, "true") == 0)
		{
			c->svcdaemon = 1;
		}
	}
	else if ((strcasecmp(s, "server") == 0) && (strcasecmp(n, "wait") == 0))
	{
		if (!v)
		{
			return(0);
		}
		c->waittime = stringtoint(v);
		if ((c->waittime < 2) || (c->waittime > 3600))
		{
			c->waittime = 60;
			return(0);
		}
	}
	else if ((strcasecmp(s, "server") == 0) && (strcasecmp(n, "srvid") == 0))
	{
		if (!v)
		{
			return(0);
		}
	}
	else
	{
		return(0);
	}

	return(1);
}

short fexist(const char *fname)
{
	struct stat *sb;

	sb = (struct stat *)malloc(sizeof(struct stat));
	if (!sb) { return(-1); }

	if (stat(fname, sb) == 0)
	{
		free(sb);
		return(1);
	}
	else
	{
		free(sb);
		return(0);
	}
}
