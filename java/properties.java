
package subdir;
import java.util.Properties;

public class properties {
	public static void main(String args[]) throws Exception {
		Properties props = new Properties();
		props.load(System.in);
		props.put("foo", "bar");
		props.list(System.out);
		System.out.println("-- End of properties listing --");

		String line = System.getProperty("banana");
		System.out.println(line);
	}
}

