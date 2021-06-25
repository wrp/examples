
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));


#[cfg(test)]
mod tests {
	use super::*;

	#[test]
	fn test_sum_2() {
		unsafe {
			let result = sum_2_int(3, 8);
			assert_eq!(result, 11);
		}
	}
}
