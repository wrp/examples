
.PHONY: all properties hello
all: hello properties

properties: william/properties.class
	printf 'baz=qux' | java -cp .:snakeyaml-1.9.jar william.properties

hello: hello.class
	java hello $(USER)

hello.class: hello.java
	javac hello.java

snakeyaml-1.9.jar.zip:
	curl -Ss http://www.java2s.com/Code/JarDownload/snakeyaml/snakeyaml-1.9.jar.zip > snakeyaml-1.9.jar.zip

snakeyaml-1.9.jar: snakeyaml-1.9.jar.zip
	unzip snakeyaml-1.9.jar.zip
	touch snakeyaml-1.9.jar  # Update timestamp

william/properties.class: properties.java snakeyaml-1.9.jar
	javac -cp snakeyaml-1.9.jar -d . properties.java

clean:
	rm -rf william
	find . -name '*.class' -delete
	rm -f snakeyml-1.9.jar*

jdb: hello.class
	@# Set breakpoint: stop at hello:4
	@# run: run, step, next, cont
	java -agentlib:jdwp=transport=dt_socket,server=y,address=8394 hello &
	rlwrap -H ~/.jdbhistory -z pipeto jdb -attach 8394

.PHONY: clean all jdb
