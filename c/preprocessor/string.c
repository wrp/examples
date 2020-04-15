
#define string(x)  #x
#define cat(a,b) a##b

string(foo)  # Expands to "foo"

cat(foo,bar) # Expands to foobar
