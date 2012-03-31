#!/usr/bin/perl
#
# Crimson Pyramid Utilities
#
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
# Usage: mkemailpw.pl <password>
# Generates a password suitable for placement in the email users database (dovecot SSHA256 format)
# Generated password is not suitable for use in Crimson Pyramid users database!
#
# Note that `doveadm -s SSHA256 pw` is more secure, and it does not require the plaintext password
# to be sent via the command line.  Using this script means that the plaintext password may show up
# in `ps` lists and your shell's history file.  It should not be considered secure.  
# This code is provided primarily for the purpose of implementing the email password storage format
# (dovecot SSHA256) in Perl.  
#

use MIME::Base64;
use Digest::SHA;

MAIN:
{
	if (!defined($ARGV[0]))
	{
		print STDOUT "PW not specified\n";
		exit(1);
	}

	$npw = &encodepass($ARGV[0]);
	print STDOUT "$npw\n";

	exit(0);
}

sub encodepass
{
        my($numA, $numB, $numC, $numD);
        my($opw, $npw, $salt, $hash);
        $opw = shift;

        $numA = int(rand(10));
        $numB = int(rand(10));
        $numC = int(rand(10));
        $numD = int(rand(10));
        $salt = $numA . $numB . $numC . $numD . '';

        $hash = Digest::SHA::sha256($opw . $salt);
        $npw = '{SSHA256}' . MIME::Base64::encode($hash . $salt, '');

        return($npw);
}
