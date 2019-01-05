
.PHONY: password passwd
passwd: password
password:
	@perl -E '@s = (A..Z,a..z,0..9); $$s .= $$s[rand @s] for 1..12; say $$s' \
	| tee /dev/tty | if env <&- | grep -q TMUX; then tmux load-buffer - && \
	echo "^^ copied to tmux paste buffer"; fi
