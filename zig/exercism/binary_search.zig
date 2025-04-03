

const use_pointer_arithmetic = true;
pub fn binarySearch(comptime T: type, target: T, const_items: []const T) ?usize {
	var items = const_items;
	var base: usize = 0;
	if (use_pointer_arithmetic) {
		while (items.len > 0) {
			const mid = items[items.len / 2 ..].ptr;
			if (mid[0] == target) {
				return base + items.len / 2;
			}
			if (mid[0] < target) {
				const right = items.len - items.len / 2;
				base += items.len / 2 + 1;
				items = mid[1..right];
			} else {
				items = items[0..items.len/2];
			}
		}
	} else {
		var left: usize = 0;
		var right = items.len;

		while (left < right) {
			const mid = left + (right - left) / 2;
			if (items[mid] == target) {
				return mid;
			} else if (items[mid] < target) {
				left = mid + 1;
			} else {
				right = mid;
			}
		}
	}
	return null;
}
