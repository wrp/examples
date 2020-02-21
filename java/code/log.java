/* Example using log4j */

/* Check the log config in config/log4j for a sample that causes
 * debug and info to go to stdout, warn and error go to stderr.
 * Note that if we uncomment the BasicConfigurator.configure()
 * invocation, cruft will still go to stdout.  There's probably
 * something we could do to prevent that... maybe?
 * */

import org.apache.log4j.Logger;
import org.apache.log4j.ConsoleAppender;
import org.apache.log4j.PatternLayout;
import org.apache.log4j.Level;
import org.apache.log4j.varia.LevelRangeFilter;

public class log{


	static Logger log = Logger.getLogger(log.class.getName());

	public static void main(String[] args) {
/*		BasicConfigurator.configure();*/
/*
log4j.appender.stdout = org.apache.log4j.ConsoleAppender
log4j.appender.stdout.Threshold = TRACE
log4j.appender.stdout.Target   = System.out
log4j.appender.stdout.layout = org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern = %-5p %d [%t][%F:%L] : %m%n
log4j.appender.stdout.filter.filter1=org.apache.log4j.varia.LevelRangeFilter
log4j.appender.stdout.filter.filter1.levelMin=TRACE
log4j.appender.stdout.filter.filter1.levelMax=INFO

log4j.appender.stderr = org.apache.log4j.ConsoleAppender
log4j.appender.stderr.Threshold = WARN
log4j.appender.stderr.Target   = System.err
log4j.appender.stderr.layout = org.apache.log4j.PatternLayout
log4j.appender.stderr.layout.ConversionPattern = %m%n
*/
		ConsoleAppender stdout = new ConsoleAppender();
		stdout.setThreshold(Level.TRACE);
		stdout.setLayout(new PatternLayout( new String ("%m%n")));
		LevelRangeFilter infoFilter = new LevelRangeFilter();
		infoFilter.setLevelMin(Level.TRACE);
		infoFilter.setLevelMax(Level.INFO);
		infoFilter.setAcceptOnMatch(true);
		stdout.addFilter(infoFilter);
		stdout.activateOptions();
		Logger.getRootLogger().addAppender(stdout);

		ConsoleAppender stderr = new ConsoleAppender();
		stderr.setThreshold(Level.WARN);
		stderr.setLayout(new PatternLayout( new String ("%m%n")));
		stderr.setTarget("System.err");
		stderr.activateOptions();
		Logger.getRootLogger().addAppender(stderr);

		for(int i=0; i < 1000; i++) {
			log.debug("Hello this is a debug message");
			log.info("Hello this is an info message");
			log.warn("Hello this is an warn message");
			log.error("Hello this is an error message");
		}
	}
}
