#!/bin/bash
#
# Crimson Pyramid Build Script
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

# Modify these for your system as necessary
CC=`which cc`
CFLAGS='-O'
LFLAGS='-L/usr/lib/mysql -O'
LIBS='inilib/ini.o -lmysqlclient -lz -lm'

set +e

if [ ! -f inilib/ini.o ] ; then
  (cd inilib && ./mk.sh)
fi

cflist='agent config updates'
oflist=''
for cfile in $cflist ; do
  if [ ! -f "${cfile}.c" ] ; then
    echo "Your package is missing ${cfile}.c and cannot build."
    exit 1
  fi
  echo "${CC} -pipe ${CFLAGS} -o ${cfile}.o -c ${cfile}.c"
  ${CC} -pipe ${CFLAGS} -o ${cfile}.o -c ${cfile}.c
  oflist="${oflist}${cfile}.o "
done
echo "${CC} -pipe ${LFLAGS} -o dnsagent ${oflist} ${LIBS}"
${CC} -pipe ${LFLAGS} -o dnsagent ${oflist} ${LIBS}

exit 0
