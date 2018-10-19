
import java.util.Arrays;

/* Trivial examples using arrays. */

public class array {

	private static void func(String arr[]) {
		int i;
		/* Iterate over the array */
		for( i=0; i < arr.length; i++) {
			System.out.println(String.format("%d: %s", i, arr[i]));
		}

		/* Check if "foo" is in the array */
		if(Arrays.asList(arr).contains("foo")) {
			System.out.println("Foo is found");
		}
	}


	public static void main(String args[]) {
		String ar[] = { "one", "two" };
		func(args);
		func(ar);
		func(new String[]{ "a", "b" });
	}
}
