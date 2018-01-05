
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


f = fn
	x,y when x > 0 -> x + y 
	x, y -> x * y
end

IO.puts f.(1,3)
IO.puts f.(-1,3)
