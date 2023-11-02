
/*

From section 6.4.6, paragraph 3 of n1256.pdf, the
2007 draft standard:

In all aspects of the language, the six tokens
<: :> <% %> %: %:%:
behave, respectively, the same as the six tokens
[   ]  {  } #   ##
*/
%:include <stdio.h>

int
main(void)
<%
	char *a<::> = <% "Hello", "world" %>;
	printf("%s, %s!\n", a<:0:>, a<:1:>);
	return 0;
%>
