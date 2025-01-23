

fn find_index_of_min(data: &[i32]) ->  usize {
	let mut min = data[0];
	let mut index: usize = 0;

	for i in 1..data.len() {
		if data[i] < min {
			min = data[i];
			index = i;
		}
	}
	return index;
}

fn swap(data: &mut[i32], a: usize, b: usize) {
	(data[a], data[b]) = (data[b], data[a]);
}

fn insertion_sort(data: &mut[i32]) {
	for d in 0..data.len() - 1 {
		let i = find_index_of_min(&data[d..]) + d;
		swap(data, i, d);
	}
}

fn main() {
	let mut a = [1, 5, 3, 2, 4];
	println!("unsorted = {:?}", a);
	insertion_sort(&mut a);
	println!("sorted   = {:?}", a);
}
