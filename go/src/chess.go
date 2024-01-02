// Read moves and display.  (Not a game engine, or a rule
// checker, just a naive display.)

// TODO:
// implement a stack to allow undo
// do some actual move validation with rule checks
// Allow abbreviations.  eg, 'Nf3' should find a knight
//    that can move to f3 and deduce the move
// Enforce 'x'.  eg, 'e3d4' should be illegal and
//    should instead be 'e3xd4' or 'pxd4' or just 'xd4'

package main

import (
	"bufio"
	"errors"
	"fmt"
	"os"
)

// TODO add color to pieces and background
type game struct {
	board map[string]square
	history []move
}

type color int
const (
	white = iota
	black
)

type rank rune
const (
	pawn = '\u2659'
	rook = '\u2656'
	knight = '\u2658'
	bishop = '\u2657'
	king = '\u2654'
	queen = '\u2655'
)

type piece struct {
	c color
	r rank
}

type square struct {
	c color
	p piece
	d string
}


func(g *game) reset() {
	g.board = map[string]square {
		"a8": square{white, piece{black, rook}, "a8"},
		"b8": square{black, piece{black, knight}, "b8"},
		"c8": square{white, piece{black, bishop}, "c8"},
		"d8": square{black, piece{black, queen}, "d8"},
		"e8": square{white, piece{black, king}, "e8"},
		"f8": square{black, piece{black, bishop}, "f8"},
		"g8": square{white, piece{black, knight}, "g8"},
		"h8": square{black, piece{black, rook}, "h8"},
		"a7": square{black, piece{black, pawn}, "a7"},
		"b7": square{white, piece{black, pawn}, "b7"},
		"c7": square{black, piece{black, pawn}, "c7"},
		"d7": square{white, piece{black, pawn}, "d7"},
		"e7": square{black, piece{black, pawn}, "e7"},
		"f7": square{white, piece{black, pawn}, "f7"},
		"g7": square{black, piece{black, pawn}, "g7"},
		"h7": square{white, piece{black, pawn}, "h7"},
		"a6": square{white, piece{}, "a6"},
		"b6": square{black, piece{}, "b6"},
		"c6": square{white, piece{}, "c6"},
		"d6": square{black, piece{}, "d6"},
		"e6": square{white, piece{}, "e6"},
		"f6": square{black, piece{}, "f6"},
		"g6": square{white, piece{}, "g6"},
		"h6": square{black, piece{}, "h6"},
		"a5": square{black, piece{}, "a5"},
		"b5": square{white, piece{}, "b5"},
		"c5": square{black, piece{}, "c5"},
		"d5": square{white, piece{}, "d5"},
		"e5": square{black, piece{}, "e5"},
		"f5": square{white, piece{}, "f5"},
		"g5": square{black, piece{}, "g5"},
		"h5": square{white, piece{}, "h5"},
		"a4": square{white, piece{}, "a4"},
		"b4": square{black, piece{}, "b4"},
		"c4": square{white, piece{}, "c4"},
		"d4": square{black, piece{}, "d4"},
		"e4": square{white, piece{}, "e4"},
		"f4": square{black, piece{}, "f4"},
		"g4": square{white, piece{}, "g4"},
		"h4": square{black, piece{}, "h4"},
		"a3": square{black, piece{}, "a3"},
		"b3": square{white, piece{}, "b3"},
		"c3": square{black, piece{}, "c3"},
		"d3": square{white, piece{}, "d3"},
		"e3": square{black, piece{}, "e3"},
		"f3": square{white, piece{}, "f3"},
		"g3": square{black, piece{}, "g3"},
		"h3": square{white, piece{}, "h3"},
		"a2": square{white, piece{white, pawn}, "a2"},
		"b2": square{black, piece{white, pawn}, "b2"},
		"c2": square{white, piece{white, pawn}, "c2"},
		"d2": square{black, piece{white, pawn}, "d2"},
		"e2": square{white, piece{white, pawn}, "e2"},
		"f2": square{black, piece{white, pawn}, "f2"},
		"g2": square{white, piece{white, pawn}, "g2"},
		"h2": square{black, piece{white, pawn}, "h2"},
		"a1": square{black, piece{white, rook}, "a1"},
		"b1": square{white, piece{white, knight}, "b1"},
		"c1": square{black, piece{white, bishop}, "c1"},
		"d1": square{white, piece{white, queen}, "d1"},
		"e1": square{black, piece{white, king}, "e1"},
		"f1": square{white, piece{white, bishop}, "f1"},
		"g1": square{black, piece{white, knight}, "g1"},
		"h1": square{white, piece{white, rook}, "h1"},
	}
	g.history = make([]move, 0)
}

func enable_reverse_video() {
	fmt.Printf("\x1b\x5b\x37\x6d")
}
func disable_reverse_video() {
	fmt.Printf("\x1b\x5b\x6d\x0f")
}

func (s square) print() {
	r := ' '
	if s.p.r != 0 {
		r = rune(s.p.r)
	}

	defer disable_reverse_video()
	if (s.c == white && s.p.r == 0) {
		enable_reverse_video()
	}
	if (s.p.c == black) {
		r += 6
	}
	fmt.Printf("%c ", r)
}

func (g game) draw() {
	for rank := '8'; rank >= '1'; rank -= 1 {
		for col := 'a'; col <= 'h' ; col += 1 {
			idx := string(col) + string(rank)
			g.board[idx].print()
		}
		fmt.Println(" ", string(rank))
	}
	fmt.Printf("a b c d e f g h\n")
	for i, t := range(g.history) {
		c := "\t";
		if (i % 2 == 1) {
			c = ("\n")
		} else {
			fmt.Printf("%d: ", i / 2 + 1)
		}
		fmt.Printf("%s", t.text)
		fmt.Printf(c)
	}
	fmt.Printf("\n\n")
}
type move struct {
	text string
	to, from string  // eg "a2"
	captured, moved piece
	player color
}

func (m *move) validate() (e error){
	switch {
	case m.from[0] < 'a' || m.from[0] > 'h':
		return errors.New("bad source row")
	case m.from[1] < '1' || m.from[1] > '8':
		return errors.New("bad source col")
	case m.to[0] < 'a' || m.to[0] > 'h':
		return errors.New("bad destination row")
	case m.to[1] < '1' || m.to[1] > '8':
		return errors.New("bad destination col")
	}
	return
}
func (m *move) parse (s string) (e error) {
	if (len(s) < 4) {
		return errors.New("invalid entry(too short)")
	}
	m.from = s[0:2]
	m.to = s[2:4]
	m.text = s
	e = m.validate()

	return
}

// undo the last move
func (g *game) undo() (e error) {
	if (len(g.history) == 0) {
		return errors.New("No previous entry")
	}
	m := g.history[len(g.history)-1]
	moved_piece := g.board[m.to].p
	to := g.board[m.to]
	to.p = m.captured
	g.board[m.to] = to

	from := g.board[m.from]
	from.p = moved_piece
	g.board[m.from] = from

	return
}

func (g *game) apply(m move) (e error) {
	src := g.board[m.from].p
	if (src.r == 0) {
		return errors.New("No piece at source")
	}

	to := g.board[m.to]
	m.captured = to.p
	to.p = src
	g.board[m.to] = to

	from := g.board[m.from]
	from.p = piece{}
	g.board[m.from] = from
	return
}

func read_move(g *game, p string) (e error){
	// Read move and update board
	switch p {
	case "u":
		if e = g.undo(); e == nil {
			g.history = g.history[0:len(g.history)-1]
		}
		return
	case "q":
		os.Exit(0)
	case "reset":
		g.reset()
		return
	}
	m := move{player: white}
	if (len(g.history) % 2 == 1) {
		m.player = black
	}
	e = m.parse(p)
	if e != nil {
		return
	}
	if g.board[m.from].p.r == 0 {
		return errors.New("No piece at " + m.from)
	}
	if g.board[m.from].p.c != m.player {
		player_color := "white"
		if m.player == black {
			player_color = "black"

		}
		return errors.New("piece at " + m.from +
			" does not belong to " + player_color)
	}

	if e == nil {
		e = g.apply(m)
	}
	if e == nil {
		g.history = append(g.history, m)
	}
	return e
}


func main() {
	var g game;
	g.reset()
	g.draw()

	s := bufio.NewScanner(os.Stdin)
	for s.Scan() {
		if err := read_move(&g, s.Text()); err == nil {
			g.draw()
		} else {
			fmt.Fprintf(os.Stderr, "%s\n", err.Error())
		}
	}
}
