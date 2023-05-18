

use std::io::Error;
use std::time::Instant;

fn main() -> Result<(), Error> {
    let now = Instant::now();

    println!("{}", "hello, world!");

    let elapsed = now.elapsed().as_micros();
    println!("Took {} microseconds", elapsed);
    Ok(())
}
