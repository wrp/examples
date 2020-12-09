#if 0
Sample input

acc +9
acc -2
acc -12
acc +33
jmp +301
#endif

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct instruction {
	enum {acc, jmp, nop} cmd;
	int arg;
	int called;
};

struct program {
	struct instruction *start, *end;
	size_t capacity;
	int accumulator;
	int ip;
};

void * xrealloc(void *buf, size_t num, size_t siz, void *endvp);
void push(struct program *b, const struct instruction *i);

int
main(void)
{
	struct program p = {0};
	struct instruction i = {0};
	char buf[4];
	while( 2 == scanf("%3s %d ", buf, &i.arg) ){
		if( !strcmp(buf, "acc") ){
			i.cmd = acc;
		} else if( !strcmp(buf, "jmp") ){
			i.cmd = jmp;
		} else if( !strcmp(buf, "nop") ){
			i.cmd = nop;
	 	} else {
			errx(EXIT_FAILURE, "inavlid cmd: %s", buf);
		}

		push(&p, &i);
	}
	while( 1 ){
		struct instruction *ip = p.start + p.ip++;
		if( ip->called++ ){
			printf("%d\n", p.accumulator);
			return 0;
		}
		switch( ip->cmd ){
		case acc: p.accumulator += ip->arg; break;
		case jmp: p.ip += ip->arg - 1; break;
		case nop: break;
		}
	}
	return 1;
}

void *
xrealloc(void *buf, size_t num, size_t siz, void *endvp)
{
	void **endp = endvp;
	ptrdiff_t offset = endp && *endp ? *endp - buf : 0;
	buf = realloc(buf, num * siz);
	if( buf == NULL ){
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	if( endp != NULL ){
		*endp = buf + offset;
	}
	return buf;
}

void
push(struct program *b, const struct instruction *i)
{
	while( b->start == NULL || b->end >= b->start + b->capacity ){
		b->capacity = b->capacity ? b->capacity + 128 : 128;
		b->start = xrealloc(b->start, b->capacity, sizeof *b->start,
			&b->end);
	}
	*b->end++ = *i;

}
