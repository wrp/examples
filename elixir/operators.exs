
IO.puts "Hello, world!"
IO.puts "Hello, " <> "concatenated" <> " world!"    # String contatenation

false and raise("Never raised")   # and, or, and not require booleans
                                  # &&, ||, and ! operate on any type

1 && IO.puts("&& operator allows non-booleans")
0 && IO.puts("'false' and 'nil' are only non-true value")
!0 || IO.puts("Negate with !")

# Sort order is number < atom < reference < function < port < pid < tuple < map < list < bitstring


{ a, b } = { :hello, :world }
# IO.puts a + b  Need to figure this out


# Guards in a function definition
f = fn
	x,y when x > 0 -> x + y 
	x, y -> x * y
end

IO.puts f.(1,3)
IO.puts f.(-1,3)


x = 5
cond do

	false -> "never true"
	true -> IO.puts "cond value always true"
end

if true do
	a = 1 + 1
	x = 5
end

# is equivalent to

if true, do: ( a = 1 + 1; x = 5)

map = %{:a => 1, 2 => :b}
IO.puts map[2]
IO.puts map[:a]
IO.puts map.a


# Functions
defmodule Funcs do
	def foo(4) do
		IO.puts "f called with 4"
	end
	def foo(x) when is_integer(x) do
		IO.puts "f called with other number"
	end
	def foo(x,y) do
		IO.puts "f called with 2 values"
	end
end

Funcs.foo(5)
f = &Funcs.foo/1
f.(4)
Funcs.foo(2,3)

p = &IO.puts/1
p.("test")
