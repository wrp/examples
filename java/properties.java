
package william;
import java.util.Properties;
import java.io.FileInputStream;

public class properties {
	public static void main(String args[]) throws Exception {
		Properties props = new Properties();
		props.load(new FileInputStream(args[0]));


		System.out.println("hello from properties");
	}
}

