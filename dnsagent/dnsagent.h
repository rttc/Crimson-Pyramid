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

#define VERSION				2012040801

#undef CONFIG_FILE

#define QUERYLEN			1024

#define stringtoint(x)			(int)strtol((x), (char **)NULL, 10)
#define stringtolong(x)			strtol((x), (char **)NULL, 10)
#define stringtouint(x)			(unsigned int)strtoul((x), (char **)NULL, 10)
#define stringtoulong(x)		strtoul((x), (char **)NULL, 10)
#define stringtofloat(x)		strtof((x), (char **)NULL)

extern unsigned short verbosity;
extern unsigned int options;

#define OPTION_DAEMON			0x001
#define OPTION_TERMINATE		0x100

#define dlvl(x)				(verbosity >= (x))
#define rundaemon			(options & OPTION_DAEMON)
#define terminating			(options & OPTION_TERMINATE)

struct _confData
{
	char dbname[128];
	char dbhost[128];
	int dbport;
	char dbuser[64];
	char dbpass[QUERYLEN];
	char dbfile[512];
	char dbcapath[512];

	short svcdaemon;
	char svcsrvid[16];
	int waittime;

	char ncf_masters[512];
	char ncf_slaves[512];
	char ncf_acls[512];
	char ncd_masters[512];
	char ncd_slaves[512];
};
typedef struct _confData Config;

/* agent.c */

/* config.c */
extern Config *read_config(const char *filename);
extern void free_config(Config *c);

/* updates.c */
extern void run_updates(int srvid);

