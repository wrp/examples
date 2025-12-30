# Put this file in ~/CLAUDE.md to set preferences on new sessions

When displaying code, render tabs with 8 spaces of visual indentation.

At session start, check if 'cat -A' is supported.  Remember the result and never retry if it fails.

Do not use 'git add -A'.  Ever.  Explicitly show me the files you are staging.
Don't assume "origin" exists.  Check the repo; if assuming, assume "github."

Don't show the /terminal-setup tip.

Write in the imperative mood with "now" referring to the parent commit's tree.
Ensure trailers are valid (all at the end, no empty lines between them).
Summary under 50 characters.
Body lines under 72 characters.
No non-ASCII characters.
No metadata prefixes like "feat:" in subjects — use trailers instead (e.g., Type: feature).


Terminal display — Tabstops every 8 columns.
Python code — Avoid inline comments; prefer extracting functions with docstrings instead.
Database naming — Use singular table names.
Comment banners — Never use =, <, or > (looks like merge conflict markers).  Use hyphens, asterisks, or pound signs.
Terminology — Say "directory," never "folder."
Sentence spacing — Two spaces after a period.

C-style:
Always use 1TBS.  Put braces around single line loop bodies.
Never use pre/post-in/decrement operators purely for the side effect.
eg, always write "for (int i = 0; i < 5; i += 1)" instead of using "i++".
