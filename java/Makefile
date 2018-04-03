
hello.class: hello.java
	javac hello.java


william/properties.class: properties.java
	javac -d properties.java

run: hello.class
	java -cp . hello
	java william.properties



clean:
	find . -name '*.class' -delete


.PHONY: clean run
