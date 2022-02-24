

use std::thread;
use std::time::Duration;


fn main() {
	println!("sleeping for 2 seconds");
	thread::sleep(Duration::from_secs(2));
}

