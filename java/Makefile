
.PHONY: all properties hello array
all: hello properties array

properties: subdir/properties.class
	printf 'baz=qux' | java -cp .:snakeyaml-1.9.jar subdir.properties

hello: hello.class
	java hello $(USER)

yaml: yaml.class
	@if test -t 0; then \
		printf 'foo: "bar"\nbaz: "qux"\n'; \
	else \
		cat; \
	fi \
	| java -cp .:snakeyaml-1.9.jar yaml

array: array.class
	java array

log: log.class
	java -Dlog4j.configuration=file:$(PWD)/config/log4j \
		-cp '.:/usr/share/java/log4j-1.2.jar:/usr/share/java/log4j-core-2.11.1.jar' log

%.class: %.java snakeyaml-1.9.jar
	javac -cp .:/usr/share/java/log4j-1.2.jar:snakeyaml-1.9.jar $<

snakeyaml-1.9.jar.zip:
	curl -Ss http://www.java2s.com/Code/JarDownload/snakeyaml/snakeyaml-1.9.jar.zip > snakeyaml-1.9.jar.zip

snakeyaml-1.9.jar: snakeyaml-1.9.jar.zip
	unzip snakeyaml-1.9.jar.zip
	touch snakeyaml-1.9.jar  # Update timestamp

subdir/properties.class: properties.java snakeyaml-1.9.jar
	javac -cp snakeyaml-1.9.jar -d . properties.java

clean:
	rm -rf subdir
	find . -name '*.class' -delete
	rm -f snakeyml-1.9.jar*

jdb: hello.class
	@# Set breakpoint: stop at hello:4
	@# run: run, step, next, cont
	java -agentlib:jdwp=transport=dt_socket,server=y,address=8394 hello &
	rlwrap -H ~/.jdbhistory -z pipeto jdb -attach 8394

.PHONY: clean all jdb
