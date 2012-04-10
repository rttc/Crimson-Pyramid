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
# description: dnsagent is the Crimson Pyramid DNS Agent
# processname: dnsagent
# config: /etc/dnsagent.ini


# Source function library
if [ -f /etc/rc.d/init.d/functions ]; then
. /etc/rc.d/init.d/functions
elif [ -f /etc/init.d/functions ]; then
. /etc/init.d/functions
elif [ -f /etc/rc.d/functions ]; then
. /etc/rc.d/functions
fi

Bin=/usr/local/sbin/dnsagent
Cfg=/etc/dnsagent.ini
LockFile=/var/lock/subsys/dnsagent

# See how we were called.
case "$1" in
  start)
	# Start daemons.
	echo -n "Starting dnsagent: "
	daemon $Bin -d
	echo
	touch $LockFile
	;;
  stop)
	# Stop daemons.
	echo -n "Shutting down dnsagent: "
	killproc dnsagent
	echo
	rm -f $LockFile
	;;
  restart)
	$0 stop
	$0 start
	;;
  status)
	status dnsagent
	;;
  *)
	echo "Usage: dnsagent {start|stop|restart|status}"
	exit 1
esac

exit 0
