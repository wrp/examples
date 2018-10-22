/* Example using log4j */

/* Check the log config in config/log4j for a sample that causes
 * debug and info to go to stdout, warn and error go to stderr.
 * Note that if we uncommend the BasicConfigurator.configure()
 * invocation, cruft will still go to stdout.  There's probably
 * something we could do to prevent that... maybe?
 * */

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
