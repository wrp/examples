

#include "print_info.h"
#include <stdio.h>

void
print_info(const struct Info *info)
{
	printf("float=%.2f int=%d str=%s\n", info->f, info->n, info->s);
}
