package Interface

import (
	"fmt"
	"math"
	"reflect"
	"strconv"
)

func Example() {
	r := rect{width: 3, height: 4}
	c := circle{radius: 5}
	z := cube{side: 4}
	var w geo2d
	var y geo3d
	w = z
	y = w.(geo3d) // type assertion

	measure(r)
	measure(c)
	measure(z)
	measure(w)
	measure(y)
}


func ToString(any interface{}) string {
	if v, ok := any.(Stringer); ok {
		return v.string()
	}
	switch v := any.(type) {
	case int:
		return strconv.Itoa(v)
	}
	return "???"
}

type geo3d interface {
	geo2d
	volume() float64
}

type geo2d interface {
	area() float64
	perim() float64
}

type rect struct{ width, height float64 }

func (r rect) area() float64  { return r.width * r.height }
func (r rect) perim() float64 { return 2*r.width + 2*r.height }

type circle struct{ radius float64 }

func (c circle) area() float64  { return math.Pi * c.radius * c.radius }
func (c circle) perim() float64 { return 2 * math.Pi * c.radius }

type cube struct{ side float64 }

func (c cube) area() float64   { return c.side * c.side * 6 }
func (c cube) perim() float64  { return c.side * 12 }
func (c cube) volume() float64 { return c.side * c.side * c.side }
func (c cube) string() string { return fmt.Sprintf("A cube with side %v", c.side) }

func measure(g geo2d) {
	if v, ok := g.(Stringer); ok {
		fmt.Printf("%s: %s %s\n",
			v.string(),
			reflect.TypeOf(g),
			reflect.ValueOf(&g).Elem().Interface(),
		)
	} else {
		fmt.Println(g)
		fmt.Println(g.area())
		fmt.Println(g.perim())
	}
	if _, ok := g.(geo3d); ok {
		fmt.Printf("value %v of type %T implements geo3d\n", g, g)
	} else {
		fmt.Printf("value %v of type %T does not implement geo3d\n", g, g)
	}
}
type Stringer interface {
	string() string
}
