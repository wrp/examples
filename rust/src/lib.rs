
pub fn public_function() {
    println!("called misc's `public_function()`");
}

fn private_function() {
    println!("called misc's `private_function()`");
}

pub fn indirect_access() {
    print!("called misc's `indirect_access()`, that\n\t> ");

    private_function();
}

