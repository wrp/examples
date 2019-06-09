struct ring_buf;
struct ring_buf * rb_create( size_t );
int rb_push( struct ring_buf *R, unsigned char c );
int rb_pop( struct ring_buf *R );
