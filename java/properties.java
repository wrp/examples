
package william;
import java.util.Properties;
import java.io.FileInputStream;
import org.yaml.snakeyaml.Yaml;
import java.util.Map;
import java.util.HashMap;

public class properties {
	public static void main(String args[]) throws Exception {
		Map<Object,Object> v = new HashMap<Object, Object>();
		Properties props = new Properties();
		props.load(System.in);
		props.put("foo", "bar");
		props.list(System.out);

		v.put(6,9);
		System.out.println(v.get(6));

		Yaml yaml = new Yaml();
		String doc = "\nfoo: bar\nbaz: qux\n";
		System.out.println(yaml.load(doc));

		Map obya = (Map)yaml.load(doc);
		System.out.println( obya.get("foo"));

		Map obyak = (Map)yaml.load(new FileInputStream("sample.yaml"));
		System.out.println( obyak.get("foo"));
	}
}

