

import java.lang.StringBuilder;

public class hello {
	String name;

	hello() {
		name = String.format("Doug");
	}
	hello(String lname) {
		name = lname;
	}
	void run() {
		System.out.println(String.format("hello %s", name));
	}
}
