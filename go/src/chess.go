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
	"strings"
	"os"
)

// TODO add color to pieces and background
type game struct {
	board [8][8]square
	history []string
}

type location struct {
	row, col int
}

type color int
const (
	white = iota
	black
)

type rank rune
const (
	pawn = 'p'
	rook = 'r'
	knight = 'n'
	bishop = 'b'
	king = 'k'
	queen = 'q'
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
	g.board = [8][8]square {
		{
		square{white, piece{black, rook}, "a8"},
		square{black, piece{black, knight}, "b8"},
		square{white, piece{black, bishop}, "c8"},
		square{black, piece{black, queen}, "d8"},
		square{white, piece{black, king}, "e8"},
		square{black, piece{black, bishop}, "f8"},
		square{white, piece{black, knight}, "g8"},
		square{black, piece{black, rook}, "h8"},
		},
		{
		square{black, piece{black, pawn}, "a7"},
		square{white, piece{black, pawn}, "b7"},
		square{black, piece{black, pawn}, "c7"},
		square{white, piece{black, pawn}, "d7"},
		square{black, piece{black, pawn}, "e7"},
		square{white, piece{black, pawn}, "f7"},
		square{black, piece{black, pawn}, "g7"},
		square{white, piece{black, pawn}, "h7"},
		},
		{
		square{white, piece{}, "a6"},
		square{black, piece{}, "b6"},
		square{white, piece{}, "c6"},
		square{black, piece{}, "d6"},
		square{white, piece{}, "e6"},
		square{black, piece{}, "f6"},
		square{white, piece{}, "g6"},
		square{black, piece{}, "h6"},
		},
		{
		square{black, piece{}, "a5"},
		square{white, piece{}, "b5"},
		square{black, piece{}, "c5"},
		square{white, piece{}, "d5"},
		square{black, piece{}, "e5"},
		square{white, piece{}, "f5"},
		square{black, piece{}, "g5"},
		square{white, piece{}, "h5"},
		},
		{
		square{white, piece{}, "a4"},
		square{black, piece{}, "b4"},
		square{white, piece{}, "c4"},
		square{black, piece{}, "d4"},
		square{white, piece{}, "e4"},
		square{black, piece{}, "f4"},
		square{white, piece{}, "g4"},
		square{black, piece{}, "h4"},
		},
		{
		square{black, piece{}, "a3"},
		square{white, piece{}, "b3"},
		square{black, piece{}, "c3"},
		square{white, piece{}, "d3"},
		square{black, piece{}, "e3"},
		square{white, piece{}, "f3"},
		square{black, piece{}, "g3"},
		square{white, piece{}, "h3"},
		},
		{
		square{white, piece{white, pawn}, "h2"},
		square{black, piece{white, pawn}, "g2"},
		square{white, piece{white, pawn}, "f2"},
		square{black, piece{white, pawn}, "d2"},
		square{white, piece{white, pawn}, "e2"},
		square{black, piece{white, pawn}, "f2"},
		square{white, piece{white, pawn}, "g2"},
		square{black, piece{white, pawn}, "h2"},
		},
		{
		square{black, piece{white, rook}, "a1"},
		square{white, piece{white, knight}, "b1"},
		square{black, piece{white, bishop}, "c1"},
		square{white, piece{white, queen}, "d1"},
		square{black, piece{white, king}, "e1"},
		square{white, piece{white, bishop}, "f1"},
		square{black, piece{white, knight}, "g1"},
		square{white, piece{white, rook}, "h1"},
		},
	}
	g.history = make([]string, 0)
}

func enable_reverse_video() {
	fmt.Printf("\x1b\x5b\x37\x6d")
}
func disable_reverse_video() {
	fmt.Printf("\x1b\x5b\x6d\x0f")
}

func (s square) print() {
	r := string(' ')

	if (s.c == white) {
		enable_reverse_video()
	}
	if (s.p.r != 0) {
		r = string(s.p.r)
	}
	if (s.p.c == white) {
		r = strings.ToUpper(r)
	}
	fmt.Printf("%s", r)
	disable_reverse_video()
}

func (g game) draw() {
	for row := 0; row < 8; row += 1 {
		for col := 0; col < 8; col += 1 {
			g.board[row][col].print()
		}
		fmt.Println()
	}
	for i, t := range(g.history) {
		c := "\t";
		if (i % 2 == 1) {
			c = ("\n")
		} else {
			fmt.Printf("%d: ", i / 2 + 1)
		}
		fmt.Printf("%s", t)
		fmt.Printf(c)
	}
	fmt.Printf("\n")
}
type move struct {
	text string
	to, from location;
}
func (m *move) parse (s string) (e error) {
	m.from.row = (int(s[0]) - 'a')
	m.from.col = 7 - (int(s[1]) - '0' - 1)
	m.to.row = (int(s[2]) - 'a')
	m.to.col = 7 - (int(s[3]) - '0' - 1)

	m.text = s

	if m.from.row < 0 || m.from.row > 7 {
		e = errors.New("bad source row")
	}
	if m.from.col < 0 || m.from.col > 7 {
		e = errors.New("bad source col")
	}
	if m.to.row < 0 || m.to.row > 7 {
		e = errors.New("bad destination row")
	}
	if m.to.col < 0 || m.to.col > 7 {
		e = errors.New("bad destination col")
	}
	return
}

func (g *game) update(m move) (e error) {
	src := g.board[m.from.col][m.from.row].p
	if (src.r == 0) {
		return errors.New("No piece at source")
	}
	g.board[m.to.col][m.to.row].p = src
	g.board[m.from.col][m.from.row].p = piece{}

	g.history = append(g.history, m.text)
	return
}

func read_move(g *game, p string) (e error){
	// Read move and update board
	if (len(p) < 4) {
		e = errors.New("invalid entry(too short)")
		return
	}
	if (p == "quit") {
		os.Exit(0)
	}
	if (p == "reset") {
		g.reset()
		return
	}
	var m move
	e = m.parse(p)

	if e == nil {
		e = g.update(m)
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
