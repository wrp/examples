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

type board map[string]square

// TODO add color to pieces and background
type game struct {
	board
	history []move
}

type color int
const (
	white = iota + 1
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
	color
	rank
}

type square struct {
	color
	piece
}


func(g *game) reset() {
	g.board = map[string]square {
		"a8": square{white, piece{black, rook}},
		"b8": square{black, piece{black, knight}},
		"c8": square{white, piece{black, bishop}},
		"d8": square{black, piece{black, queen}},
		"e8": square{white, piece{black, king}},
		"f8": square{black, piece{black, bishop}},
		"g8": square{white, piece{black, knight}},
		"h8": square{black, piece{black, rook}},
		"a7": square{black, piece{black, pawn}},
		"b7": square{white, piece{black, pawn}},
		"c7": square{black, piece{black, pawn}},
		"d7": square{white, piece{black, pawn}},
		"e7": square{black, piece{black, pawn}},
		"f7": square{white, piece{black, pawn}},
		"g7": square{black, piece{black, pawn}},
		"h7": square{white, piece{black, pawn}},
		"a6": square{white, piece{}},
		"b6": square{black, piece{}},
		"c6": square{white, piece{}},
		"d6": square{black, piece{}},
		"e6": square{white, piece{}},
		"f6": square{black, piece{}},
		"g6": square{white, piece{}},
		"h6": square{black, piece{}},
		"a5": square{black, piece{}},
		"b5": square{white, piece{}},
		"c5": square{black, piece{}},
		"d5": square{white, piece{}},
		"e5": square{black, piece{}},
		"f5": square{white, piece{}},
		"g5": square{black, piece{}},
		"h5": square{white, piece{}},
		"a4": square{white, piece{}},
		"b4": square{black, piece{}},
		"c4": square{white, piece{}},
		"d4": square{black, piece{}},
		"e4": square{white, piece{}},
		"f4": square{black, piece{}},
		"g4": square{white, piece{}},
		"h4": square{black, piece{}},
		"a3": square{black, piece{}},
		"b3": square{white, piece{}},
		"c3": square{black, piece{}},
		"d3": square{white, piece{}},
		"e3": square{black, piece{}},
		"f3": square{white, piece{}},
		"g3": square{black, piece{}},
		"h3": square{white, piece{}},
		"a2": square{white, piece{white, pawn}},
		"b2": square{black, piece{white, pawn}},
		"c2": square{white, piece{white, pawn}},
		"d2": square{black, piece{white, pawn}},
		"e2": square{white, piece{white, pawn}},
		"f2": square{black, piece{white, pawn}},
		"g2": square{white, piece{white, pawn}},
		"h2": square{black, piece{white, pawn}},
		"a1": square{black, piece{white, rook}},
		"b1": square{white, piece{white, knight}},
		"c1": square{black, piece{white, bishop}},
		"d1": square{white, piece{white, queen}},
		"e1": square{black, piece{white, king}},
		"f1": square{white, piece{white, bishop}},
		"g1": square{black, piece{white, knight}},
		"h1": square{white, piece{white, rook}},
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
	if s.piece.rank != 0 {
		r = rune(s.piece.rank)
	}
	b := rune(s.piece.rank)
	w := b + 6

	defer disable_reverse_video()
	if (s.color == white) {
		b, w = w, b;
		enable_reverse_video()
	}
	switch s.piece.color {
	case white: r = w
	case black: r = b
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
	moved_piece := g.board[m.to].piece
	to := g.board[m.to]
	to.piece = m.captured
	g.board[m.to] = to

	from := g.board[m.from]
	from.piece = moved_piece
	g.board[m.from] = from

	return
}

func (g *game) apply(m move) (e error) {
	src := g.board[m.from].piece
	if (src.rank == 0) {
		return errors.New("No piece at source")
	}

	to := g.board[m.to]
	m.captured = to.piece
	to.piece = src
	g.board[m.to] = to

	from := g.board[m.from]
	from.piece = piece{}
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
	if g.board[m.from].piece.rank == 0 {
		return errors.New("No piece at " + m.from)
	}
	if g.board[m.from].piece.color != m.player {
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
