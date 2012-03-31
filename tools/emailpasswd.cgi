#!/usr/bin/perl
#
# Crimson Pyramid Email Password CGI
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
# This script enables users to change their email password from your webmail interface
# such as Roundcube.  This can be used with the Crimson Pyramid Roundcube mod.  
#

#
# The following options must be set in order to connect to the database
# which must be named 'email'.  
#
$DBun = 'username';
$DBpw = 'password';
$DBho = 'localhost';

use CGI;
use DBI;
use Time::HiRes;
use MIME::Base64;
use Digest::SHA;

MAIN:
{
	local($c, $db);
	my($dbinfo, $username, $opass, $npass, $query, $apass, $uid, @ema);

	$c = CGI->new();

	$username = $c->param('user');
	if (!defined($username) || (length($username) < 6))
	{
		&respond(1, "Failure: Invalid submission.");
		exit(0);
	}
	if (!($username =~ /.+\@([a-zA-Z0-9-]+)*\.[a-zA-Z]+/))
	{
		&respond(1, "Failure: Invalid username");
		exit(0);
	}
	if ($username =~ /[\;\"\'\\]/)
	{
		&respond(1, "Failure: Invalid username");
		exit(0);
	}
	$opass = $c->param('opass');
	if (!defined($opass) || (length($opass) < 1))
	{
		&respond(1, "Failure: Invalid submission");
		exit(0);
	}
	$npass = $c->param('npass');
	if (!defined($npass) || (length($npass) < 1))
	{
		&respond(1, "Failure: Invalid submission");
		exit(0);
	}
	if ((length($npass) < 6) || !($npass =~ /[a-zA-Z]/) || !($npass =~ /[0-9]/))
	{
		&respond(1, "Failure: New password is too weak.");
		exit(0);
	}
	if ($npass =~ /[\;\"\'\\]/)
	{
		&respond(1, "Failure: New password contains invalid characters: [' \" ; \\].");
		exit(0);
	}

	$dbinfo = 'DBI:mysql:database=email;host=' . $DBho;
	$db = DBI->connect($dbinfo, $DBun, $DBpw);
	if (!defined($db))
	{
		&respond(0, "Database connection failed!  Try again.");
		exit(1);
	}

	@ema = split(/\@/, $username, 2);
	$uid = &getuid($ema[0], $ema[1]);

	if ($uid < 0)
	{
		&respond(1, "User \"" . $ema[0] . '@' . $ema[1] . "\" not found in database!");
		$db->disconnect();
		exit(0);
	}

	if (&verifypass($uid, $opass) <= 0)
	{
		&respond(1, "Password verification failed.");
		$db->disconnect();
		exit(0);
	}

	$apass = &encodepass($npass);

	$query = 'update mail_users set passwd=' . $db->quote($apass) . " where id=$uid";
	$db->do($query);
	$query = 'update mail_users set lastchg=' . time() . " where id=$uid";
	$db->do($query);

	$db->disconnect();
	&respond(2, "Success.");

	exit(0);
}

sub respond
{
	my($code, $rtext, $class);
	$code = shift;
	$rtext = shift;

	$class = 'pwerror';
	if ($code == 0)
	{
		$class = 'pwerror';
	}
	elsif ($code == 1)
	{
		$class = 'pwfailure';
	}
	elsif ($code == 2)
	{
		$class = 'pwsuccess';
	}

	print STDOUT "Content-type: text/html\n\n";
	print STDOUT "<div id=\"pwcresp\"><p class=\"" . $class . "\">" . $rtext . "</p></div>\n";

	return(0);
}

sub getuid
{
	my($a, $b, $query, $u);
	$a = shift;
	$b = shift;

	$a =~ s/\\//g;
	$a =~ s/\"//g;
	$a =~ s/\;//g;
	$a =~ s/\'//g;
	$b =~ s/\\//g;
	$b =~ s/\"//g;
	$b =~ s/\;//g;
	$b =~ s/\'//g;

	$query = "select id from mail_users where username=\"$a\" and domain=\"$b\"";
	$res = $db->prepare($query);
	$res->execute();
	if ($res->rows > 0)
	{
		my($rhash);
		$rhash = $res->fetchrow_hashref();
		$u = $rhash->{'id'};
	}
	else
	{
		$u = -1;
	}

	$res->finish();
	return($u);
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

	#$npw = '{SSHA256}' . MIME::Base64::encode($hash, '') . $salt;
	$npw = '{SSHA256}' . MIME::Base64::encode($hash . $salt, '');

	return($npw);
}

sub verifypass
{
	my($id, $opw);
	$id = shift;
	$opw = shift;

	# /* verify that opw validates to the user's current password */

	return(1);
}
