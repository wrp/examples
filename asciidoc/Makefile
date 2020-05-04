
%.html: %.txt ; asciidoctor -b xhtml5 $<

%.xml: %.txt ; asciidoctor -b docbook $<

# Total lie: foo.1 can depend on bar.txt.  The name of the file
# is irrelevant, as *.txt will generate n.1 where n depends only
# on the content of the file, not the name.
%.1: %.txt ; asciidoctor -a name=the-name -a version=0.0.1 -b manpage $<


view:
	groff -man -Tascii the-name.1
