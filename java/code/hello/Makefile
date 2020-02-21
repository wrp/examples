

run: hello.class main.class superhello.class
	JAVA_TOOL_OPTIONS="-Dhello.name='Name set in Makefile'" \
	java main

%.class: %.java
	javac $<
