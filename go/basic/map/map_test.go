
package maps

import "testing"

func TestXXX(t *testing.T) {
	k := map[int]int{
		1: 10,
		2: 12,
		5: 13,
	}
	map_incr(k)
	if k[1] != 11 {
		t.Errorf("k[1] did not increment.  Got %v", k[1])
	}
}
