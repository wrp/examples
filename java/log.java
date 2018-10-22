/* Example using log4j */

import org.apache.log4j.Logger;
import org.apache.log4j.BasicConfigurator;

public class log{


	static Logger log = Logger.getLogger(log.class.getName());
   
	public static void main(String[] args) {
/*		BasicConfigurator.configure();*/
		log.debug("Hello this is a debug message");
		log.info("Hello this is an info message");
		log.warn("Hello this is an warn message");
		log.error("Hello this is an error message");
	}
}
