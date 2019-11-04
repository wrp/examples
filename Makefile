
.PHONY: password passwd
passwd: password
password:
	@perl -E '@s = (A..Z,a..z,0..9); $$s .= $$s[rand @s] for 1..12; say $$s' \
	| { while read line; do echo "$$line" >&3; echo "$$line"; done \
	| if env <&- | grep -q TMUX; then tmux load-buffer - && \
	echo "^^ copied to tmux paste buffer" >&2; fi; } 3>&1 \
	| if test -f /proc/self/fd/2; then tee /proc/self/fd/2; else tee /dev/stderr; fi \
	| if pbcopy 2> /dev/null; then echo "^^ copied to clipboard" >&2; fi;
