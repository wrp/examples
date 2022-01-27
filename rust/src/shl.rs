



use std::ops::Shl;

#[allow(non_camel_case_types)]
struct shlVec<T>(Vec<T>);

impl<T> Shl<T> for shlVec<T> {
	type Output = shlVec<T>;

	fn shl(mut self, elem: T) -> Self::Output {
		self.0.push(elem);
		self
	}
}

fn main() {
	let mut v = shlVec(vec![1, 2, 3]);
	v = v << 4;
	assert_eq!(vec![1, 2, 3, 4], v.0)
}
