ifelse(`
Example taken fom https://stackoverflow.com/questions/69325405/gnu-m4-seems-to-access-unnecessary-value-in-ifelse
')dnl
define(`for',
`ifelse(
        `$#', `2', `for(`$1', `$2', `1')',
        eval(`$1' + ifelse(`$3', `', `0', `1') > `$2'), `1', `$1',
        `$1, for(eval(`$1' + `$3'), `$2', `$3')'dnl
)')dnl

for(3,9)
for(2,11,2)
for(2,10,2)
