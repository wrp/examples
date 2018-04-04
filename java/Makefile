
all: hello.class william/properties.class
	java -cp . hello
	printf 'baz=qux' | java william.properties



hello.class: hello.java
	javac hello.java

snakeyaml-1.9.jar.zip:
	curl http://www.java2s.com/Code/JarDownload/snakeyaml/snakeyaml-1.9.jar.zip > snakeyaml-1.9.jar.zip

snakeyaml-1.9.jar: snakeyaml-1.9.jar.zip
	unzip snakeyaml-1.9.jar.zip

william/properties.class: properties.java snakeyaml-1.9.jar
	javac -cp snakeyaml-1.9.jar -d . properties.java

clean:
	rm -rf william
	find . -name '*.class' -delete


.PHONY: clean all
