
hello.class: hello.java
	javac hello.java


william/properties.class: properties.java
	javac -d . properties.java

run: hello.class william/properties.class
	java -cp . hello
	java william.properties /dev/null



clean:
	rm -rf william
	find . -name '*.class' -delete


.PHONY: clean run
