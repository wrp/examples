
public class hello extends superhello {
	String name;

	hello() {
		String default_name = System.getProperty("hello.name", "Doug");
		this.name = String.format(default_name);
	}
	hello(String lname) {
		name = lname;
	}
	void run() {
		System.out.println(String.format("hello %s, %s", name, supername));
	}
}
