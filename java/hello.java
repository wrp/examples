
import java.io.FileInputStream;

public class hello {
	public static void main(String args[]) {
		System.out.println(String.format("hello %s", "world"));
		System.out.println(
			/* hello.getClass() */
			hello.class
			.getClassLoader()
			.getResource("hello.class")
		);

		/* Read name from a file */
		try {
			FileInputStream f = new FileInputStream(args[0]);
			int c;
			while(( c = f.read()) != -1) {
				System.out.print((char)c);
			}
		}
		catch( Exception e) {
			System.err.print(e + "\n");
		}
	}
}
