
#define concat(x) push_##x
#define push(x) concat(x)
static void
push(T)(int c, T *b)
{
	if( b->start == NULL || b->end >= b->start + b->cap ) {
		b->start = xrealloc(b->start, b->cap += 128,
			sizeof *b->start, &b->end);
	}
	*b->end++ = c;
}
#undef T
#undef push
#undef concat
