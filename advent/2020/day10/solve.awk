#!/bin/sh

sort -n input | awk '1 END{print $0+3}' | awk 'BEGIN{a[0]=0; b[0]= 1}
	{a[NR] = $0}
	{ for(i=1; i<=3; i++) {if(a[NR]-a[NR-i] < 4 && NR >= i) b[NR] += b[NR-i] }}
	END { print b[NR]}'
