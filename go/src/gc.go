// Demonstrate garbage collection tuning
// keywords: gc garbage collection SetGCPercent SetMemoryLimit
//
// debug.SetGCPercent(pct) controls how often the GC runs based on
// heap growth.  The default is 100, meaning the GC triggers when
// the heap has doubled since the last collection.  Setting it to
// -1 disables GC entirely (except for explicit runtime.GC() calls).
// The return value is the previous setting.
//
// debug.SetMemoryLimit(limit) sets a soft cap on total runtime memory.
// When approaching the limit, the GC runs more aggressively regardless
// of SetGCPercent.  It will not crash or fail allocations if the limit
// is exceeded; live data that needs more memory will exceed it.  The
// default is math.MaxInt64 (effectively unlimited).
//
// A common pattern is SetGCPercent(-1) with SetMemoryLimit(N) to get
// predictable memory-bounded behavior: no GC based on growth ratio,
// but GC kicks in when approaching the memory cap.
package main

import (
	"fmt"
	"runtime"
	"runtime/debug"
)

func heapMB() float64 {
	var m runtime.MemStats
	runtime.ReadMemStats(&m)
	return float64(m.HeapAlloc) / (1 << 20)
}

func allocate(n int) [][]byte {
	s := make([][]byte, n)
	for i := range s {
		s[i] = make([]byte, 1<<20) // 1 MiB each
	}
	return s
}

func main() {
	fmt.Printf("heap: %.1f MiB (baseline)\n", heapMB())

	// Disable GC and allocate
	old := debug.SetGCPercent(-1)
	_ = allocate(50)
	fmt.Printf("heap: %.1f MiB (after 50 MiB alloc, GC disabled)\n", heapMB())

	// Re-enable GC and force a collection
	debug.SetGCPercent(old)
	runtime.GC()
	fmt.Printf("heap: %.1f MiB (after GC re-enabled and forced)\n", heapMB())

	// Demonstrate SetMemoryLimit: disable ratio-based GC,
	// but set a 30 MiB soft limit so GC runs to stay under it
	debug.SetGCPercent(-1)
	debug.SetMemoryLimit(30 << 20)
	for i := 0; i < 10; i++ {
		_ = allocate(5)
		fmt.Printf("heap: %.1f MiB (iteration %d, 30 MiB soft limit)\n", heapMB(), i)
	}
}
