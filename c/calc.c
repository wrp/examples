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
#define commands "kpq"

struct state {
	double *stack, *sp;
	size_t stack_size;
	char *buf, *bp;
	size_t buf_size;
	char fmt[32];
};

void process_entry( struct state *S, int c );
void apply_operator(struct state *S, int c);
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
		write_args_to_stdin(argv + 1);
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

/*
 * Decrement the stack pointer.  This checks if the caller
 * can consume the given number of arguments, and leaves
 * the stack pointer pointed to assume the caller will replace
 * repl arguments on the stack.  (ie, this simply decrements
 * the stack pointer by consume - repl, but does a bounds check first).
 */
void
decr(struct state *S, int repl, int consume)
{
	assert( S->sp >= S->stack );
	assert( consume >= repl );
	if( S->sp - S->stack < consume ) {
		fprintf(stderr, "Stack empty (need %d values)\n", consume);
	} else {
		S->sp -= consume - repl;
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
	}
	if( S->sp - S->stack == S->stack_size ) {
		grow_stack(S);
	}
}


void
apply_command(struct state *S, int c)
{
	switch(c) {
	case 'k':
		decr(S, 0, 1);
		snprintf(S->fmt, sizeof S->fmt, "%%.%dg\n", (int)S->sp[0]);
		break;
	case 'p': {
		decr(S, 1, 1);
		printf(S->fmt, S->sp[-1]);
	} break;
	case 'q': exit(0);
	default: assert(0); /* no coverage */
	}
}


void
apply_operator(struct state *S, int c)
{
	switch(c) {
	case '*': decr(S, 1, 2); S->sp[-1] *= S->sp[0]; break;
	case '+': decr(S, 1, 2); S->sp[-1] += S->sp[0]; break;
	case '/': decr(S, 1, 2); S->sp[-1] /= S->sp[0]; break;
	case '-': decr(S, 1, 2); S->sp[-1] -= S->sp[0]; break;
	case '^': decr(S, 1, 2); S->sp[-1] = pow(S->sp[-1], S->sp[0]); break;
	default: assert(0); /* no coverage */
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
		perror("realloc"); /* no coverage */
		exit(1);           /* no coverage */
	}
	return rv;
}

void
die(const char *msg)  /* no coverage */
{
	perror(msg);  /* no coverage */
	exit(1);      /* no coverage */
}

void xpipe(int *fd) { if(pipe(fd) == -1) die("pipe"); }
int xdup2(int s, int t) { if(dup2(s,t) == -1)  die("dup2"); xclose(s); }
void xclose(int fd) { if(close(fd) == -1 ) die("close"); }
void write_args_to_stdin(const char **argv)
{
	int p[2];
	xpipe(p);
	xdup2(p[0],STDIN_FILENO);
	switch(fork()) {
	case -1: die("fork"); /* no coverage */
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
