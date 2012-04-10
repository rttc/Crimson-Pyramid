#!/bin/sh
#
# Crimson Pyramid Init Script
# Copyright (C) 2012 Matt Harris
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#
# chkconfig: 2345 90 10
# description: sqlagent is the Crimson Pyramid SQL DB Agent
# processname: sqlagent
# config: /etc/sqlagent.ini


# Source function library
if [ -f /etc/rc.d/init.d/functions ]; then
. /etc/rc.d/init.d/functions
elif [ -f /etc/init.d/functions ]; then
. /etc/init.d/functions
elif [ -f /etc/rc.d/functions ]; then
. /etc/rc.d/functions
fi

Bin=/usr/local/sbin/sqlagent
Cfg=/etc/sqlagent.ini
LockFile=/var/lock/subsys/sqlagent

# See how we were called.
case "$1" in
  start)
	# Start daemons.
	echo -n "Starting sqlagent: "
	daemon $Bin -d
	echo
	touch $LockFile
	;;
  stop)
	# Stop daemons.
	echo -n "Shutting down sqlagent: "
	killproc sqlagent
	echo
	rm -f $LockFile
	;;
  restart)
	$0 stop
	$0 start
	;;
  status)
	status sqlagent
	;;
  *)
	echo "Usage: sqlagent {start|stop|restart|status}"
	exit 1
esac

exit 0
