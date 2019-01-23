
die() { test -n "$*" && echo "$*" >&2; exit 1; }

interactive_commit() {
	local response
	msg="$1"
	if git diff --cached | grep .; then
		if test -z "$FORCE"; then
			printf "%s ([n]yR)? " "Commit the above changes" > /dev/tty
			read response < /dev/tty
			case $response in
			R)
				echo "enter commit msg.  ^D to terminate"
				msg=$(cat < /dev/tty)  # msg is global
				;;
			y|Y|yes|YES)
				;;
			*)
				msg="";;
			esac
		fi
		if test -n "$msg"; then
			git commit -m "$msg"
		else
			git reset; echo resetting repo, not committing
		fi
	fi
}
