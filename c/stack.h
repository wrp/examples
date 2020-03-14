
#define  _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <stdlib.h>
struct stack;
struct stack * stack_create(size_t);
struct stack * stack_xcreate(size_t);
int stack_push(struct stack *, void *);
int stack_pop(struct stack *, void *);
void * stack_get(struct stack *, int);
void * stack_incr(struct stack *);
void * stack_decr(struct stack *);
void * stack_base(struct stack *);
size_t stack_size(struct stack *);
