LEN += 12

.PHONY: password passwd
passwd: password
password:
	@perl -E 'my $$k = ""; my @s = ("A".."N","P".."Z","a".."k","m".."z",2..9); $$k .= $$s[rand @s] for 1..$(LEN); say $$k' \
	| while read line; do \
		printf '%s\n' "$$line" >&2; \
		if env <&- | grep -q TMUX; then \
			printf '%s' "$$line" | tmux load-buffer - && \
			echo "^^ copied to tmux paste buffer" >&2; \
		fi; \
		if echo "$$line" | xclip -selection clipboard 2> /dev/null; then echo "^^ copied to clipboard" >&2; fi; \
	done
