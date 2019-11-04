#!/usr/bin/env perl

# Replace occurences of 'mm/dd/yy HH:MM:SS' with epoch time

use strict;
use warnings;
use Time::Piece;

while(<>) {
	s@(\d{1,2}/\d{1,2}/\d\d \d{1,2}:\d\d:\d\d)@
		sprintf scalar Time::Piece->strptime(
			$1, "%m/%d/%y %H:%M:%S")->epoch@ge;
	print;
}

