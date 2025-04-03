#!/usr/bin/env perl

use 5.12.10;
use Time::Piece;

# Increment an ISO 8601 time (eg 2022-08-20T02:08:15Z) by 1 second
while( <> ){
	if( $_ !~ m@\d{4}-\d{1,2}-\d{1,2}T\d{1,2}:\d{2}:\d{2}Z@ ){
		say STDERR "Invalid format $_";
		next
	}
	chop;  # Delete newline
	chop;  # Delete trailing 'Z'
	my $t = Time::Piece->strptime($_, '%Y-%m-%dT%H:%M:%S');
	say $t + 1;
}

exit;


# Replace occurences of 'mm/dd/yy HH:MM:SS' with epoch time
while(<>) {
	s@(\d{1,2}/\d{1,2}/\d\d \d{1,2}:\d\d:\d\d)@
		sprintf scalar Time::Piece->strptime(
			$1, "%m/%d/%y %H:%M:%S")->epoch@ge;
	print;
}


# shell:
# three_days_ago=$( perl -MTime::Piece -MTime::Seconds=ONE_DAY -E 'my $g=gmtime; $g -= 3* ONE_DAY; say $g->datetime' )
# echo 2022-08-20T02:08:59Z | perl -MTime::Piece -lnE 'chop; my $t = Time::Piece->strptime($_, "%Y-%m-%dT%H:%M:%S") + 1; say $t->datetime'
#
# find the last weekday of the previous month:
#printf '2023-01-07\n2023-01-07\n' |
#perl -MTime::Piece -nE '
#	@k=(2,3,1,1,1,1,1);
#	$d = Time::Piece->strptime($_, "%Y-%m-%d\n")->truncate(to=>"month")+1; # one second past midnight on first day of this month
#	$p = $d-$k[$d->_wday]*24*60*60; say "$d -> $p; " . $d->_wday'
