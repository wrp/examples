
package subdir;
import java.util.Properties;

/* Properties can be set by -D flags on command line,
 * are set by the system (eg java.vm.info),
 * taken from JAVA_TOOL_OPTIONS (
 * 	eg JAVA_TOOL_OPTIONS="-Dlog4j.configuration=file:/p/a/t/h -Dfoo=bar"),
 * and probably other sources
 */
public class properties {
	public static void main(String args[]) throws Exception {
		Properties props = new Properties();
		props.load(System.in);
		props.put("foo", "bar");
		props.list(System.out);
		System.out.println("-- End of properties listing --");

		String line = System.getProperty("banana");
		System.out.println(line);

		Properties jvm = System.getProperties();
	        jvm.list(System.out);
	}
}

