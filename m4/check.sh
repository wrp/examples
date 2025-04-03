#!/bin/sh
# Check if a file defines a macro
# usage: ./$0 macro file [file ...]

# Fails if any of the files listed do not define macro

macro=$1
shift
for x; do
{
cat "$x"
cat << EOF
ifdef(\`$macro',,
\`errprint(fatal: \`\`$macro'' undefined in \`\`$x''
)m4exit(1)')
EOF
} | m4 > /dev/null
done
