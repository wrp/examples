// Demonstrate derived std::fmt::Debug

#[derive(Debug)]
#[allow(dead_code)]
struct Rectangle {
    width: u32,
    height: u32,
    name: String,
}

#[derive(Debug)]
struct Point ( i32, i32, i32 );  // A tuple-struct

// Use "Field Init Shorthand
fn demo_field_init( height: u32, width: u32, name: String) -> Rectangle { Rectangle { height, width, name } }


fn main() {
    let rect1 = Rectangle { width: 30, height: 50, name: "Bob".to_string() };
    let rect2 = demo_field_init(5, 10, String::from("Amy"));

    let a = Point(2, 4, 5);
    println!("point is {:?}.  Middle is {}", a, a.1);

    // Use struct update.  Since all the copied fields implement the copy trait, this
    // does not borrow rect1
    let rect3 = Rectangle { name: String::from("Copied"), ..rect1 };

    println!("rect1 is {:?}", rect1);
    println!("rect1 is {:?}", rect2);
    println!("rect1 is {:?}", rect3);
    println!("height is {:#?}", rect1.height);
}
