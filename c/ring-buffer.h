struct ring_buf;
struct ring_buf * rb_create( size_t );
int rb_push( struct ring_buf *R, unsigned char c );
int rb_pop( struct ring_buf *R );
int rb_tail( struct ring_buf *R );
int rb_isempty( struct ring_buf *R );
int rb_string(struct ring_buf *, char *, size_t);
int rb_length(struct ring_buf *);
