// Find the contiguous subarray with highest sum
// Demonstrate Copy trait

// #[derive(Copy, Clone)]
struct Arr {
	sum: i32,
	start: usize,
}

// Instead of deriving, can implement the trait easily
impl Copy for Arr {}
impl Clone for Arr { fn clone(&self) -> Self { *self } }

fn main() {
        let v = vec![1, -2, 2, 3, -10, 3, 17, -1, 13, -1500, 5];
	let mut cur = Arr {sum: 0, start: 0};
	let mut max = Arr {sum: 0, start: 0};

	for (i, x) in v.iter().enumerate() {
		if *x > 0 {
			cur.sum += x;
		} else if i32::abs(*x) < cur.sum {
			cur.sum += x;
		} else {
			if cur.sum > max.sum {
				max = cur;
			}
			cur.sum = 0;
			cur.start = i + 1;
		}

	}
	println!("arr: {:?}", v);
	println!("sum: {}, start: {}", max.sum, max.start);
}
