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
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>
#include <sys/syslog.h>
#include "dnsagent.h"

struct _ACLEnt
{
	unsigned int nent;
	char **ent;
};
struct _ACLList
{
	unsigned int nacl;
	char **acls;
	struct _ACLEnt **entries;
};

typedef struct _ACLList ACLList;

static ACLList *gather_acls(void);
static char **gather_domains(int sid);

void run_updates(int srvid)
{
	char **domains;
	ACLList *acls;

	acls = gather_acls();
	if (acls)
	{
		print_acls(acls);
	}

	return;
}

ACLList *gather_acls()
{
	char query[QUERYLEN];
	ACLList *al;

	al = (ACLList *)malloc(sizeof(ACLList));
	if (!al)
	{
		return((ACLList *)NULL);
	}
	
}

char **gather_domains(int sid)
{

}
