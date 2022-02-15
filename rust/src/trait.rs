




trait Foo  {
	fn bar(&self) -> &str;
}

impl Foo for u8 {
	fn bar(&self) -> &str { "bar for u8" }
}

fn do_foo<T: Foo>(item: T){
	println!("{}", item.bar());
}

fn main() {
	do_foo(5_u8);
}
