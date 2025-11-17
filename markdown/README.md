
This page has only been validated by looking at this file on github,
so should certainly not be considered anything but github specific.

[documentation](https://developer.statuspage.io)
Use text in square brackets followed by url in parentheses to make a link.

```
$ echo Use backticks for a code block, or indent with 4 spaces
```

    This text is indented with 4 spaces,
    so it will be a code block. Same as using backticks.

----
Create a horizontal line by placing 3 or more hyphens, asterisks,
***
or underscores on a line by themselves. All three of these look the same.
___

Level 1 and level 2 headers get a horizontal line.  Smaller
headers do not.

# This is a level 1 header
## This is level 2 header
### This is level 3 header
#### Level 4 header
##### Level 5 header
###### Level 6 header

**Double asterisk for bold text** (no space inside **)
*Single asterisk for italics* (no space inside *)
You can also use _underscores_ to get italicized word,
and __double underscore__ to get bold.

## A blockquote
> Create a
blockquote
by using > in the first column.  The quote
may span
multiple lines in the markdown but is rendered according to the available width.

## A checklist
- [ ] this is the first item
- [x] this is the selected tem
- [ ] this is the 3rd item

## An ordered list
1. First item
    - Some variants of markdown seem to require blank lines.  I don't fully understand
    that: maybe it is just our current linter.
1. Second Item
    1. Subitems should be indented with 4 spaces

## An un-ordered list
- First item
- Second Item

## Tables
To create a table, use `|` to separate columns.
The header is defined by the presence of 3 or more
hyphens in the second row.  You can control the justification
of text in the column with a colon:

| left-justified | centered | right justified |
| :---        |    :----:   |          ---: |
| Apple      | Banana       | Cherry |
| Dog | Elephant        | Frog |

~~~
Code block is delimited by triple tilde or triple back-tick
~~~

This is outside of any block.

```
This also denotes a code block.  (A "fenced code block", according to OAI's chat GPT-5)
```
