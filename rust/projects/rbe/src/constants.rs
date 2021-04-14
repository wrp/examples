// https://doc.rust-lang.org/stable/rust-by-example/custom_types/constants.html

// Globals are declared outside all other scopes.
static LANGUAGE: &str = "Rust";
const THRESHOLD: i32 = 10;

fn is_big(n: i32) -> bool { n > THRESHOLD }

fn main() {
    let n = 16;

    println!("This is {}", LANGUAGE);
    println!("The threshold is {}", THRESHOLD);
    println!("{} is {}", n, if is_big(n) { "big" } else { "small" });

    // Error! Cannot modify a `const`.
    // THRESHOLD = 5;
}

