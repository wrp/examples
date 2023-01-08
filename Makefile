LENGTH += 12

.PHONY: password passwd
passwd: password
password:
	@perl -E '@s = (A..Z,a..z,0..9); $$s .= $$s[rand @s] for 1..$(LENGTH); say $$s' \
	| while read line; do \
		printf '%s\n' "$$line" >&2; \
		if env <&- | grep -q TMUX; then \
			printf '%s' "$$line" | tmux load-buffer - && \
			echo "^^ copied to tmux paste buffer" >&2; \
		fi; \
		if echo "$$line" | pbcopy 2> /dev/null; then echo "^^ copied to clipboard" >&2; fi; \
	done
