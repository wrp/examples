struct ring_buf;
struct ring_buf * rb_create( size_t );
void rb_free(struct ring_buf *);
int rb_push( struct ring_buf *R, unsigned char c );
int rb_pop( struct ring_buf *R );
int rb_peek(struct ring_buf const *R, size_t idx);
int rb_tail( struct ring_buf *R );
int rb_isempty( struct ring_buf *R );
void rb_xpush( struct ring_buf *R, unsigned char c );
