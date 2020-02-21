/*
 * Read yaml from stdin
 */
import org.yaml.snakeyaml.Yaml;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.FileReader;
import java.io.IOException;
import java.lang.StringBuilder;

public class yaml {
	public static void main(String args[]) {
		Yaml yaml = new Yaml();
		String doc = "";

		try {
			/* BufferedReader br = new BufferedReader(new FileReader(args[0])); */
			BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
			StringBuilder sb = new StringBuilder();
			String line;
			/* there has got to be a cleaner way to read stdin into a string */
			while( null != (line = br.readLine())) {
				doc += line + "\n";
			}
		}
		catch(java.io.FileNotFoundException e) {
			System.err.print("FileNotFound:" + e + "\n");
			System.out.println( String.format("Hello, %s!", args[0]));
		}
		catch(IOException e) {
			System.err.print(e + "\n");
		}

		System.out.println(yaml.load(doc));
	}
}
