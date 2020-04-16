
%.html: %.txt ; asciidoctor -b xhtml5 $<

%.xml: %.txt ; asciidoctor -b docbook $<

%.1: %.txt ; asciidoctor -a version=0.0.1 -b manpage $<

