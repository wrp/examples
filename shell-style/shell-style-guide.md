<a id="shell-style-guide"></a>

# Shell Style Guide

### File Names

Executables should never have a filename "extension" like '.sh' or
'.bash'.  When you execute 'ls' or 'grep', you do not care if the
tool was implemented in C, Rust, Zig, or something else.  If
your shell script becomes useful, people will use it in their tools.
If you call it 'foo.sh' and then need to re-implement the functionality
in C or Rust or Zig, you cannot easily rename the tool 'foo', because
millions of other tools are already using it with the name 'foo.sh'.
It will be called 'foo.sh' in perpetuity, and people will be baffled
that a tool named 'foo.sh' is implemented in Zig.  The entire notion
of a filename "extension" is a throw-back to an ancient OS which has
no place in a modern system.  You use a shebang to indicate the
file type; that metadata has no place in the filename.  Suppose
"foo.pl", "foo.sh", "foo.py", and "foo.bash" are all links to a
file whose first 4 bytes are 7F 45 4C 46.  What type of file is it?
The suffixes in the names are just a distraction.

Libraries (files intended to be sourced) should have a `.sh`
extension, should *not* have a shebang, and should not be executable.
If your library uses bashsism or zshisms, it should have a '.bash'
suffix or '.zsh' suffix, respectively.

### Error handling

All error messages should go to `STDERR`.  All output should go to `STDOUT`.
A usage statement is not an error message.  Do not try to anticipate
why a command failed; if a command fails, it should have already
written a useful error message.  The common idiom is:


```shell
die() { printf "%s${1:+\n}" "$@"; exit 1; } >&2

cmd || die
```

It is generally a mistake to write:

```shell
if ! cmd; then
	die "I don't know why cmd failed"
fi
```

It is *always* a mistake to write `set -e` or `set -o errexit` or `#!/bin/sh -e`.
Explicit is better than implicit, and the explicit `|| die` enhances readability.
There are so many gotchas and edge cases related to errexit that it is simply
unsafe to use.

### File Header

Start each file with a description of its contents.  For executables, that
description should be used to generate the usage statement.

```shell
#!/bin/sh
# Get logs from all pods
#
# usage: @name@ [-h|--help] [-n namespace] service

usage() {
	n=$(basename $0)
	sed -E -e '/^$/q' -e 1d -e 's/^# ?//' -e "s|@name@|$n|" "$0"
	exit
}
```

### Question mark

You should almost never explicitly refer to `$?`.  It is always better
to write:

```shell
if cmd; then ...
```
than it is to write:

```shell
cmd
if test $? = 0; then ...
```

However, although `exit` and `return` are exactly equvivalent to `exit $?` and
`return $?`, it can be useful to write `exit $?` or `return $?`.  Doing
so indicates that you are relying on the status of the previous
command, and reminds the reader of the semantics.  This is a common
source of bugs, and being explicit is worthwhile.

### Test

Prefer `test` over `[`.  The symbol `[` is not part of the grammar of
the shell, and it is an extremely common source of errors.  The `[[` keyword
*is* part of the grammar, but it should be avoided.  `[[` was a well-intended
fix to many of the problems inherent in `test`, but it swallows error messages
and is therefore completely unusable.


### Implementation Comments

In general, comments should explain *why* code is doing something
rather than *what* code is doing.  However, even well written shell
code can be difficult to understand so it is advisable to
add comments to explain tricky or non-obvious parts of the code.  But
you should endeavor to make such comments unnecessary.



## Formatting

### Indentation

Indent with tabs.  The shell token '<<-' has semantics which essentially
require hard tabs to be useful, and this is such a useful device that
it mandates the use of tabs throughout your code.

### Line Length and Long Strings

Maximum line length is 80 characters, assuming tabstops at every 8 columns.
Words that are longer than 80 chars should be refactored into variables.


### Pipelines

If it doesn't comfortably fit on one line, pipelines should be split with
one simple command per line.  It should be split either with the pipe
symbol aligned with the first character of the first simple command, or
with the pipe symbol at the end of each line with an extra level of
indentation:

```shell
command1 | command2

command1 \
| command2 \
| command3 \
| command4

command1 |
	command 2 | # this is a comment
	command 3
```



### Control Flow

For single line commands, put `; then` and `; do` on the same line as the `if`, `for`, or `while`
For multi-line commands, put do/then on its own line with the same indentation as the if/for/while.

Example:

```shell
if cmd; then
	do_something
fi

while
	cmd1
	cmd2
	cmd3
do
	inner_loop
done

```

### Case statement

- Each pattern should have the same indentation as the keyword "case"
- If *every* case can fit in one line, then use a space after the closing brace of the
  pattern and before the `;;` and write all branches on a single line.
- Long or multi-command alternatives should be split over multiple lines with
  the pattern, commands, and `;;` on separate lines.


```shell
case ${v} in
a) cmd1 ;;
b) cmd ;;
*) die "Unexpected: '${v}'" ;;
esac

case ${b} in
a)
	cmd1 with some arguments that make it a bit too long for a single line
	cmd2
	;;
b)
	cmd
	;;
*)
	die "Unexpected: '${b}'"
	;;
esac
```

### Variable expansion

Prefer `"${var}"` over `"$var"`.

Quote variables when appropriate, and do not overquote.  Do not quote bare-words
that do not need to be quoted.  If you need an unquoted variable, be very
explicit:

```shell
cmd $foo             # don't do this
cmd ${foo:+"$foo"}   # do this instead
```

Avoid relying on word-splitting.  eg, rather than

```shell
arg="$(conditionally assign "-f path")"
cmd $arg     # DO NOT DO THIS
```

it is much clearer to write:

```shell
arg=path
cmd ${arg:+-f "$arg"}  # This is OK
```


### Command Substitution

Use `$(command)` instead of backticks.


### Eval

`eval` should be avoided.

### Arrays

Avoid arrays.  If your code is sufficiently complex that you need
an array, then you probably shouldn't be using the shell at all.

### Pipes to While

In many shells, pipes create a subshell and any variables modified within a
pipeline do not propagate to the parent shell.  This can be a useful way
to get local variable semantics, but is a common source of confusion.  You
can help to make the code more explicit by using braces:

```shell
cmd_generating_input_to_while | while read line; do {
	cmd "$line"
} done
```

If you do *not* want the subshell, you can use a heredoc:

```shell
while read line; do { cmd "$line"; } done << EOF
$(cmd_generating_input_to_while)
EOF
```

avoid bashisms like `while read line; do ...; done < <(cmd)`


### Arithmetic

Always use `(( ... ))` or `$(( ... ))` rather than
`let` or `$[ ... ]` or `expr`.


## Aliases

Do not use them.  For decades, the documentation for bash has stated
"For almost every purpose, aliases are superseded by shell functions."
This is good advice.  Do not use aliases.



## Naming Conventions

### Function Names

Lower-case, with underscores to separate words.  Parentheses are
required after the function name.  The keyword `function` should
be avoided.

### Variable Names

Same as for function names.  Do not use all caps.  All caps names
are reserved for common environment variables like PATH and HOME;
your variables should be lower case.

## Export

Understand the semantics of export.  Use export sparingly.  If you
export a variable, you are essentially elevating its scope to
a higher plane, and readers of the code must know if/how it
is used in every single executable that will be invoked
by your program or any of its descendants.


### Use Local Variables

Declare function-specific variables with `local`.

Declaration and assignment must be separate statements when the
assignment value is provided by a command substitution.

```shell
foo() {
	local bar="$1"  # ok
	local baz=$(cmd) || return  # BAD: if cmd fails, the return is *not* executed
	local qux
	qux="$(cmd)" || return  # Good: function returns if cmd fails
}
```

### Function Location

Put all function definitions together at top of the file.  Have a function called
"main", and have a call to main be the only command at the top-level scope.  All
constants that are initialized should be done either at the top of main or
in a function that is called early in main.  eg, every script should look like:

```shell
#!/bin/sh
foo() { ... }
bar() { ... }
init() { ... }
main() { init "$@"; ... }
main "$@"
```
