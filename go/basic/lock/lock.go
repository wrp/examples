
package lock

import (
	"fmt"
	"sync"
)


var l sync.Mutex
var a string

func f() {
	a = "hello, world"
	l.Unlock()
}

func Example() {
	l.Lock()
	go f()
	l.Lock()
	fmt.Println(a)
}

/* From https://golang.org/ref/mem
For any sync.Mutex or sync.RWMutex variable l and n < m, call n of l.Unlock() happens before call m of l.Lock() returns.
Example() is guaranteed to print "hello, world". The first call to l.Unlock()
(in f) happens before the second call to l.Lock() (in main) returns,
which happens before the print.

For any call to l.RLock on a sync.RWMutex variable l, there is an
n such that the l.RLock happens (returns) after call n to l.Unlock
and the matching l.RUnlock happens before call n+1 to l.Lock.
*/

