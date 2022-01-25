// Todo: look at std::fs::OpenOptions;

use std::{env,fs::File};
use anyhow::{Context, Result};

fn count_words<R: Read>(input: &mut R) -> Result<u32, Box<dyn Error>> {
	let reader = BufReader::new(input);
	let mut wordcount = 0;
	for line in reader.lines() {
		for _word in line?.split_whitespace() {
			wordcount += 1;
		}
	}
	Ok(wordcount)
}

fn main() -> Result<()> {
    for filename in env::args().skip(1).collect::<Vec<String>>() {
        let mut reader = File::open(&filename).context(format!("unable to open '{}'", filename))?;
        let wordcount =
            count_words(&mut reader).context(format!("unable to count words in '{}'", filename))?;
        println!("{} {}", wordcount, filename);
    }
    Ok(())
}
