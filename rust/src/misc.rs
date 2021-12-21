
pub fn public_function() {
    println!("called misc's `public_function()`");
}

fn private_function() {
    println!("called miscrary's `private_function()`");
}

pub fn indirect_access() {
    print!("called miscrary's `indirect_access()`, that\n> ");

    private_function();
}

