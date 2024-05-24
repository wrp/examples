

/*
You are given two non-empty linked lists representing two non-negative
integers. The digits are stored in reverse order, and each of their
nodes contains a single digit. Add the two numbers and return the
sum as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Example 1:

Input: l1 = [2,4,3], l2 = [5,6,4]
Output: [7,0,8]
Explanation: 342 + 465 = 807.
Example 2:

Input: l1 = [0], l2 = [0]
Output: [0]
Example 3:

Input: l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
Output: [8,9,9,9,0,0,0,1]

Constraints:

The number of nodes in each linked list is in the range [1, 100].
0 <= Node.val <= 9
It is guaranteed that the list represents a number that does not have leading zeros.
*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


struct ListNode {
	int val;
	struct ListNode *next;
};


struct ListNode *
new(int val, struct ListNode *next)
{
	struct ListNode *k = malloc(sizeof *k);
	k->val = val;
	k->next = next;
	return k;
}


struct ListNode *
addTwoNumbers(struct ListNode *l1, struct ListNode *l2)
{
	int carry = 0;
	struct ListNode *k = new(0, NULL);
	struct ListNode *l = k;
	while (l1 || l2) {

		assert(l1 == NULL || (l1->val > -1 && l1->val < 10));
		assert(l2 == NULL || (l2->val > -1 && l2->val < 10));
		unsigned v = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + carry;
		assert(v < 20);
		if (v > 9) {
			carry = 1;
			v -= 10;
		} else {
			carry = 0;
		}
		l->next = new(v, NULL);
		l = l->next;
		if (l1) l1 = l1->next;
		if( l2) l2 = l2->next;
	}
	if (carry) {
		l->next = new(1, NULL);
	}
	l = k->next;
	free(k);
	return l;
}



int
main(int argc, char **argv)
{
	struct ListNode a[7], b[4];
	a[0].val = 9; a[0].next = a + 1;
	a[1].val = 9; a[1].next = a + 2;
	a[2].val = 9; a[2].next = a + 3;
	a[3].val = 9; a[3].next = a + 4;
	a[4].val = 9; a[4].next = a + 5;
	a[5].val = 9; a[5].next = a + 6;
	a[6].val = 9; a[6].next = NULL;

	b[0].val = 9; b[0].next = b + 1;
	b[1].val = 9; b[1].next = b + 2;
	b[2].val = 9; b[2].next = b + 3;
	b[3].val = 9; b[3].next = NULL;
	struct ListNode *s = addTwoNumbers(a, b);
	for (; s != NULL; s = s->next) {
		putchar(s->val + '0' );
	}
	putchar('\n');

	return 0;
}
