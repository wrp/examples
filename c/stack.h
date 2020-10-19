
#define  _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <stdlib.h>

#define STACK_RAW   0x1  /* stores/ returns void * (no copy) */
#define STACK_ALIGN 0x2  /* alway align on a page */

struct stack;
struct stack * stack_create(size_t);
struct stack * stack_xcreate(size_t);
int stack_push(struct stack *, void *);
void stack_xpush(struct stack *, void *);
void * stack_pop(struct stack *, void *);
void * stack_get(struct stack *, int);
void * stack_base(struct stack *);
size_t stack_size(struct stack *);
