
package william;
import java.util.Properties;
import java.io.FileInputStream;

public class properties {
	public static void main(String args[]) throws Exception {
		Properties props = new Properties();
		props.load(System.in);
		props.put("foo", "bar");
		props.list(System.out);
	}
}

