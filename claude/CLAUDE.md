# Put this file in ~/CLAUDE.md to set preferences on new sessions

When displaying code, render tabs with 8 spaces of visual indentation.

At session start, check if 'cat -A' is supported.  Remember the result and never retry if it fails.

Do not use 'git add -A'.  Ever.  Explicitly show me the files you are staging.
Don't assume "origin" exists.  Check the repo; if assuming, assume "github."

Don't show the /terminal-setup tip.

git specific hints:
Never use the word "sha"; commits and objects are identified by their oid.

commit messages:
	Ensure trailers are valid (all at the end, no empty lines between them).
	Write in the imperative mood with "now" referring to the parent commit's tree.
	No non-ASCII characters.
	No metadata prefixes like "feat:" in subjects — use trailers instead (e.g., Type: feature).
	Keep lines in the Body under 72 characters.
	Keep summaries under 50 characters.
	Do not add "Generated with Claude Code" or robot emojis.
	If the git config core.claude-attributions is "no" (or falsey, or unset),
		do not add any Claude-related trailers or attribution to commits.


Terminal display — Tabstops every 8 columns.
Python code — Avoid inline comments; prefer extracting functions with docstrings instead.
Database naming — Use singular table names.
Comment banners — Never use =, <, or > (looks like merge conflict markers).  Use hyphens, asterisks, or pound signs.
Terminology — Say "directory," never "folder."
Sentence spacing — Two spaces after a period.

C-style:
One declaration per statement.  Never do "int x, y;".  Always do "int x;\nint y;"
Always use 1TBS.  Put braces around single line loop bodies.
Never use pre/post-in/decrement operators purely for the side effect.
  eg, always write "for (int i = 0; i < 5; i += 1)" instead of using "i++".
Do not mention line numbers in comments.  Ever.  They become stale too quickly.

yaml style:
Do NOT increase the indentation level for list items.  eg, use:
a:
- b: 5
  c: 5


When providing assistance, always tell me how to do things from a shell rather
than a gui.  eg, instead of telling me to 'go to Settings → Resources → Advanced → "Virtual disk limit"',
tell me how to use osascript to kill the docker deamon and let me know which json
file to edit.

Do not show proposed changes larger than 10 lines or so.  If the diff is likely
to scroll off my terminal (feel free to query $LINES), I do not want you to display it.
Indeed, if any text you intend to display is likely to scroll off my screen (eg,
if it is greater than .8 * $LINES), do not show it.  Show me a summary instead.
