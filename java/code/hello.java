
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.StringBuilder;

public class hello {
	public static void main(String args[]) {
		System.out.println(String.format("hello %s", "world"));
		System.out.println(
			/* hello.getClass() */
			hello.class
			.getClassLoader()
			.getResource("hello.class")
		);

		/*
		System.out.println("version: " + hello.getClass()
			.getImplementationVersion());
			*/

		/* Read name from a file */
		try {

			InputStream fis = new FileInputStream(args[0]);
			BufferedReader br=new BufferedReader(new InputStreamReader(fis));

			/* BufferedReader br = new BufferedReader(new FileReader(args[0])); */
			StringBuilder sb = new StringBuilder();
			String t = br.readLine();
			System.out.println( String.format("Hello, %s!", t));
		}
		catch(java.io.FileNotFoundException e) {
			System.err.print("FileNotFound:" + e + "\n");
			System.out.println( String.format("Hello, %s!", args[0]));
		}
		catch(IOException e) {
			System.err.print(e + "\n");
		}
	}
}
