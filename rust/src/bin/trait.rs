




trait Foo  {
	fn bar(&self) -> String;
}

impl Foo for u8 {
	fn bar(&self) -> String { format!("bar for {}", self) }
}

fn do_foo<T: Foo>(item: T){
	println!("{}", item.bar());
}

fn main() {
	do_foo(5_u8);
}
