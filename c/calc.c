/*
 * A simple reverse polish calculator
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

#define operators "*+/^-"
#define commands "fkpq"

struct state {
	double *stack, *sp;
	size_t stack_size;
	char *buf, *bp;
	size_t buf_size;
	char fmt[32];
};

void process_entry( struct state *S, int c );
void apply_operator(struct state *S, int c);
void apply_command(struct state *S, int c);
void grow_stack( struct state *S );
void * xrealloc( void *p, size_t s );
void die(const char *msg);
void xpipe(int *fd);
int xdup2(int s, int t);
void xclose(int fd);
void write_args_to_stdin(const char **argv);
void push_number(struct state *S);

int
main(int argc, char **argv)
{
	int c;
	struct state S[1];

	S->stack_size = 4;
	S->stack = xrealloc( NULL, sizeof *S->stack * S->stack_size );
	S->sp = S->stack;
	S->buf_size = 1;
	S->buf = xrealloc( NULL, sizeof *S->buf * S->buf_size );
	S->bp = S->buf;
	strcpy(S->fmt, "%.3g\n");

	if( argc > 1) {
		write_args_to_stdin( argv + 1 );
	}
	while( (c=getchar()) != EOF ) {
		process_entry(S, c);
	}
	return 0;
}

void push_buf(struct state *S, int c)
{
	*S->bp++ = (char)c;
	if( S->bp - S->buf == S->buf_size ) {
		S->buf = xrealloc(S->buf, S->buf_size * 2 * sizeof *S->buf );
		S->bp = S->buf + S->buf_size;
		S->buf_size *= 2;
	}
}

void
process_entry(struct state *S, int c)
{
	if(strchr( " \t\n", c )) {
		push_number(S);
	} else if(strchr( operators, c )) {
		push_number(S);
		apply_operator(S, c);
	} else if(strchr( commands, c )) {
		push_number(S);
		apply_command(S, c);
	} else {
		push_buf(S, c);
	}
}


void
push_number(struct state *S)
{
	*S->bp = '\0';
	if( S->bp != S->buf ) {
		char *end;
		*(S->sp++) = strtod(S->buf, &end);
		if( end != S->bp ) {
			fprintf(stderr, "Garbled: %s\n", S->buf);
		}
		S->bp = S->buf;
		if( S->sp - S->stack == S->stack_size ) {
			grow_stack(S);
		}
	}
}


void
apply_command(struct state *S, int c)
{
	assert( S->sp >= S->stack );
	assert( strchr( commands, c ));
	if( S->sp - S->stack < 1 ) {
		fputs( "Stack empty (need 1 value)\n", stderr );
		return;
	}
	switch(c) {
	case 'k':
		snprintf(S->fmt, sizeof S->fmt, "%%.%dg\n", (int)*--S->sp);
		break;
	case 'f':
		for(double *s = S->stack; s < S->sp; s++) {
			printf("%3u: ", (unsigned)(s - S->stack));
			printf(S->fmt, *s);
		}
		break;
	case 'p': {
		printf(S->fmt, S->sp[-1]);
	} break;
	case 'q': exit(0);
	}
}


void
apply_operator(struct state *S, int c)
{
	assert( S->sp >= S->stack );
	if( S->sp - S->stack < 2 ) {
		fputs( "Stack empty (need 2 values)\n", stderr );
		return;
	}
	S->sp -= 1;
	switch(c) {
	case '*': S->sp[-1] *= S->sp[0]; break;
	case '+': S->sp[-1] += S->sp[0]; break;
	case '/': S->sp[-1] /= S->sp[0]; break;
	case '-': S->sp[-1] -= S->sp[0]; break;
	case '^': S->sp[-1] = pow(S->sp[-1], S->sp[0]); break;
	}
}


void
grow_stack( struct state *S )
{
	assert( S->sp - S->stack == S->stack_size );
	S->stack = xrealloc(S->stack, S->stack_size * 2 * sizeof *S->stack );
	S->sp = S->stack + S->stack_size;
	S->stack_size *= 2;
}


void *
xrealloc( void *p, size_t s )
{
	void *rv = realloc( p, s );
	if( rv == NULL ) {
		perror("realloc"); /* uncovered */
		exit(1);           /* uncovered */
	}
	return rv;
}

void
die(const char *msg)  /* uncovered */
{
	perror(msg);  /* uncovered */
	exit(1);      /* uncovered */
}                     /* uncovered */

void xpipe(int *fd) { if(pipe(fd) == -1) die("pipe"); }
int xdup2(int s, int t) { if(dup2(s,t) == -1)  die("dup2"); xclose(s); }
void xclose(int fd) { if(close(fd) == -1 ) die("close"); }
void write_args_to_stdin(const char **argv)
{
	int p[2];
	xpipe(p);
	xdup2(p[0],STDIN_FILENO);
	switch(fork()) {
	case -1: die("fork"); /* uncovered */
	case 0:
		xclose(STDIN_FILENO);
		xdup2(p[1], STDOUT_FILENO);
		for(; *argv; argv++ ) {
			printf("%s ", *argv);
		}
		exit(EXIT_SUCCESS);
	default:
		xclose(p[1]);
	}
}
