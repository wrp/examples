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
	int changed;
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
run:
	for( struct instruction *i = p.start; i < p.end; i++ ){
		if( i->changed == 2 ){
			switch( i->cmd ){
			case jmp: i->cmd = nop; break;
			case nop: i->cmd = jmp; break;
			case acc: break;
			}
		}
		if( i->changed == 1 ){
			i->changed = 2;
		}
		i->called = 0;
	}
	p.accumulator = 0;
	p.ip = 0;
	struct instruction *prev = NULL;
	while( p.ip < p.end - p.start ){
		struct instruction *ip = p.start + p.ip++;
		if( ip->called++ ){
			if( prev == NULL ){
				errx(1, "oops");
			}
			switch( prev->cmd ){
			case jmp: prev->cmd = nop; break;
			case nop: prev->cmd = jmp; break;
			case acc: break;
			}
			prev->changed += 1;
			goto run;
		}
		switch( ip->cmd ){
		case acc: p.accumulator += ip->arg; break;
		case jmp:
			p.ip += ip->arg - 1;
			/* Fall thru */
		case nop:
			if( ip->changed == 0 ){
				prev = ip;
			}
			break;
		}
	}
	printf("%d\n", p.accumulator);
	return 0;
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
