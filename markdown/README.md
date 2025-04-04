
This page has only been validated by looking at this file on github,
so should certainly not be considered anything but github specific.

[documentation](https://developer.statuspage.io)
Use text in square brackets followed by url in parentheses to make a link.

~~~~
$ echo Use '~~~~' for a code block
~~~~

----
Create a horizontal line by placing 3 or more hyphens, asterisks,
***
or underscores on a line by themselves. All three of these look the same.
___

# This is a 1-hash header
The single hash header is big.

## This is 2-hash header
The 2 hash header is not as big.

### This is 3-hash header
The 3 hash header is smallish, and looks a lot like **bold text**
**Double asterisk for bold text** (no space inside **)
*Single asterisk for italics* (no space inside *)

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
	This also denotes a code block.
```
