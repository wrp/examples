
password:
	@perl -E '@s = (A..Z,a..z,0..9); $$s .= $$s[rand @s] for 1..12; say $$s'
