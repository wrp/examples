// Demonstrate derived std::fmt::Debug

#[derive(Debug)]
#[allow(dead_code)]
struct Rectangle {
    width: u32,
    height: u32,
}

// Use "Field Init Shorthand
fn demo_field_init( height: u32, width: u32, ) -> Rectangle { Rectangle { height, width } }


fn main() {
    let rect1 = Rectangle { width: 30, height: 50 };
    let rect2 = demo_field_init(5, 10);

    println!("rect1 is {:?}", rect1);
    println!("rect1 is {:?}", rect2);
    println!("height is {:#?}", rect1.height);
}
