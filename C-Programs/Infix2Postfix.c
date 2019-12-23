#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define SIZE 40

/*This is a simple standard program to convert an infix expression eg.: a+b*c+d into its postfix equivalent (in this example: ab*c+d+) using the stack ADT.
This is very fundamental algorithm taught in a Data Structures course, to highlight the applications of the stack ADT.
    We follow the below algorithm for each of the element (ch) in the input infix expression string s:
        1. If ch is an operand, push to output.
        2. If ch is an operator:
            a. While stack priority of topmost element in the stack >= infix priority of ch, keep popping element from stack to output.
            b. Then push ch into stack.
        3. If ch is a '(', blindly push to stack.
        4. If ch is a ')', keep popping elements from stack till we reach a '('. Then delete the '(' without displaying it.
*/

char stack[SIZE]; //Changing stack to be of length SIZE.
char operators[] = "+-*/"; //List of recognized operators.
int top = -1;

void push(char e) {
	if (top <= SIZE - 1)
		stack[++top] = e;
}

char pop() {
	return (top > -1) ? stack[top--] : 0;
}

int isOperator(char c) {
	int i;
	for (i = 0; operators[i]; i++)
		if (operators[i] == c)
			return 1;
	return 0;
}

int stack_priority(char e) {
	switch (e) {
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
			return 2;
		default:
			return  0;
	}
}

int infix_priority(char e) {
	switch (e) {
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
			return 2;
		default:
			return  3;
	}
}

int main() {
	char *s = (char *)malloc(sizeof(char)*SIZE), ch;
	int i;

	if (!s) {
		printf("\nCouldn't find memory! Exiting...");
		return 0;
	}
	s[0] = '(';

	printf("\nEnter infix expression: ");
	fgets(s+1, SIZE, stdin);
	s[strlen(s)-1] = 0;
	strcat(s, ")");
	printf("\n\nEntered infix expression: %s \nPostfix equivalent expression: ", s);
	for (i = 0; s[i]; i++) {
		ch = s[i];
		if (isalnum(ch)) {
			printf("%c", ch);
			continue;
		}
		else if (isOperator(ch)) {
			while (stack_priority(stack[top]) >= infix_priority(ch))
				printf("%c", pop());
			push(ch);
			continue;
		}
		switch (ch) {
			case '(':
				push(ch);
				break;
			case ')':
				while ((ch = pop()) != '(')
					printf("%c", ch);
				break;
		}

	}
	free(s);

	printf("\nAllocated memory released successfully.\n\n");
	return 0;
}
