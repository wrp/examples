
package iota
import "fmt"

type ByteSize float64

const (  // enum
    _           = iota // ignore first value by assigning to blank identifier
    KiB ByteSize = 1 << (10 * iota)
    MiB
    GiB
    TiB
    PiB
    EiB
    ZiB
    YiB
)

func Example() {
	fmt.Println("iota: ", KiB, MiB, GiB, TiB, PiB)
}
