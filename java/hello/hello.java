
public class hello {
	String name;

	hello() {
		String default_name = System.getProperty("hello.name", "Doug");
		name = String.format(default_name);
	}
	hello(String lname) {
		name = lname;
	}
	void run() {
		System.out.println(String.format("hello %s", name));
	}
}
