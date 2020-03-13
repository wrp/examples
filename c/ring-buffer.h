struct ring_buf;
struct ring_buf * rb_create( size_t );
int rb_push( struct ring_buf *R, unsigned char c );
int rb_pop( struct ring_buf *R );
int rb_tail( struct ring_buf *R );
int rb_clear( struct ring_buf *R );
int rb_isempty( struct ring_buf *R );

/* Mostly temporary, for debugging */
char * rb_start( struct ring_buf *R );
char * rb_end( struct ring_buf *R );
