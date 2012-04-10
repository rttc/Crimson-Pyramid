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
static short print_acls(ACLList *al);

void run_updates(int srvid)
{
	char **domains;
	ACLList *acls;

	if (!DBhandle) { return; }

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
	MYSQL_RES *res;
	MYSQL_ROW data;
	int dnum;

	al = (ACLList *)malloc(sizeof(ACLList));
	if (!al)
	{
		return((ACLList *)NULL);
	}
	memset(al, 0, sizeof(ACLList));

	memset(query, 0, QUERYLEN);
	snprintf(query, QUERYLEN, "select * from dns_acls");
	if (mysql_query(DBhandle, query) != 0)
	{
		return((ACLList *)NULL);
	}
	res = mysql_store_result(DBhandle);
	if (!res)
	{
		return((ACLList *)NULL);
	}
	dnum = mysql_num_rows(res);
	if (dnum < 1)
	{
		return((ACLList *)NULL);
	}

	return(al);
}

char **gather_domains(int sid)
{

}

/*
describe dns_acls ; 
+---------+---------------------+------+-----+---------+-------+
| Field   | Type                | Null | Key | Default | Extra |
+---------+---------------------+------+-----+---------+-------+
| aclname | varchar(16)         | NO   | UNI | NULL    |       |
| aclid   | tinyint(3) unsigned | NO   | PRI | NULL    |       |
+---------+---------------------+------+-----+---------+-------+
2 rows in set (0.00 sec)

describe dns_acldata ; 
+-------+---------------------+------+-----+---------+-------+
| Field | Type                | Null | Key | Default | Extra |
+-------+---------------------+------+-----+---------+-------+
| aclid | tinyint(3) unsigned | NO   | MUL | NULL    |       |
| addr  | varchar(16)         | NO   |     | NULL    |       |
+-------+---------------------+------+-----+---------+-------+
2 rows in set (0.00 sec)

describe dns_domains ; 
+--------+------------------+------+-----+---------+-------+
| Field  | Type             | Null | Key | Default | Extra |
+--------+------------------+------+-----+---------+-------+
| domain | varchar(64)      | NO   | UNI | NULL    |       |
| domid  | int(10) unsigned | NO   | PRI | NULL    |       |
+--------+------------------+------+-----+---------+-------+
2 rows in set (0.00 sec)

describe dns_sd ; 
+-------+------------------+------+-----+---------+-------+
| Field | Type             | Null | Key | Default | Extra |
+-------+------------------+------+-----+---------+-------+
| domid | int(10) unsigned | NO   | MUL | NULL    |       |
| srvid | int(10) unsigned | NO   | MUL | NULL    |       |
| role  | enum('M','S')    | NO   |     | M       |       |
+-------+------------------+------+-----+---------+-------+
3 rows in set (0.00 sec)

describe dns_srv ; 
+------------+------------------+------+-----+---------+-------+
| Field      | Type             | Null | Key | Default | Extra |
+------------+------------------+------+-----+---------+-------+
| servername | varchar(64)      | NO   | UNI | NULL    |       |
| srvid      | int(10) unsigned | NO   | PRI | NULL    |       |
+------------+------------------+------+-----+---------+-------+
2 rows in set (0.00 sec)

describe dns_entries ;
+-------+------------------------------------------------+------+-----+---------+-------+
| Field | Type                                           | Null | Key | Default | Extra |
+-------+------------------------------------------------+------+-----+---------+-------+
| domid | int(10) unsigned                               | NO   | MUL | NULL    |       |
| name  | varchar(64)                                    | NO   |     | NULL    |       |
| type  | enum('A','AAAA','CNAME','TXT','MX','NS','SRV') | NO   |     | TXT     |       |
| ip4   | varchar(16)                                    | YES  |     | NULL    |       |
| ip6   | varchar(128)                                   | YES  |     | NULL    |       |
| host  | varchar(64)                                    | YES  |     | NULL    |       |
+-------+------------------------------------------------+------+-----+---------+-------+
6 rows in set (0.00 sec)

*/
