

CPPFLAGS = -I ../include

all: libxutil.a libring-buffer.a libstack.a

%.a: %.o
	ar -r $@ $<

clean:
	rm -f *.a *.o
