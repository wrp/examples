
package william;
import java.util.Properties;
import java.io.FileInputStream;
import org.yaml.snakeyaml.Yaml;

public class properties {
	public static void main(String args[]) throws Exception {
		Properties props = new Properties();
		props.load(System.in);
		props.put("foo", "bar");
		props.list(System.out);

		Yaml yaml = new Yaml();
		String doc = "\nfoo\n- bar\n- baz\n- qux\n";
		List<String> list = (List<String>) yaml.load(document);
		System.out.println(list);
	}
}

