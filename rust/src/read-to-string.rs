

use std::env;
use std::fs::read_to_string;
use std::io::{self, Error, Write};

fn main() -> Result<(), Error> {
    for filename in env::args().skip(1).collect::<Vec<String>>() {
	    let wordlist = read_to_string(filename)?;
	    let mut list: Vec<&str> = wordlist.split_ascii_whitespace().collect();

	    list.sort_unstable();
	    io::stdout().write(list.join("\n").as_bytes())?;

	    // Add trailing newline
	    print!("\n")
    }
    Ok(())
}
