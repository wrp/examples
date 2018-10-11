
public class hello {
	public static void main(String args[]) {
		System.out.println(String.format("hello %s", "world"));
		System.err.println(String.format("error"));
		System.out.println(
			/* hello.getClass() */
			hello.class
			.getClassLoader()
			.getResource("hello.class")
		);
	}
}
