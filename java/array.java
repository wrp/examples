
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.lang.StringBuilder;

/* Trivial examples using arrays. */

public class array {

	private static void func(String arr[]) {
		int i;
		for( i=0; i < arr.length; i++) {
			System.out.println(String.format("%d: %s", i, arr[i]));
		}
	}


	public static void main(String args[]) {
		String ar[] = { "one", "two" };
		func(args);
		func(ar);
		func(new String[]{ "a", "b" });
	}
}
