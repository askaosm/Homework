#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include<stdlib.h>
#define MAX_STACK_SIZE 100 
#define MAX_EXPR_SIZE 20+1

typedef enum {
	lparen, rparen, plus, minus, times, divide, mod, unary, eos, operand
}precedence;

struct stack_
{
	int top;
	int* stack;
};
struct stack_* newStack()
{
	struct stack_* pt = (struct stack_*)malloc(sizeof(struct stack_));
	pt->top = 0;
	pt->stack = (int*)malloc(sizeof(int) * MAX_STACK_SIZE);
	return pt;
}
void stackFull()
{
	fprintf(stderr, "Stack is full, cannot add element.");
	exit(1);
}

void stackEmpty()
{
	fprintf(stderr, "Stack is empty, cannot delete element.");
	exit(1);
}


void push(precedence item, struct stack_* pt) {
	if (pt->top >= MAX_STACK_SIZE - 1) stackFull();
	else pt->stack[++pt->top] = item;
}
int pop(struct stack_* pt) {
	if (pt->top == -1) stackEmpty();
	return (precedence)pt->stack[pt->top--];
}

void saveToken(precedence token, char* post, int in) {
	switch (token) {
	case lparen: post[in++] = '('; ; break;
	case rparen: post[in++] = ')'; break;
	case plus: post[in++] = '+'; break;
	case minus: post[in++] = '-'; break;
	case divide: post[in++] = '/'; break;
	case times: post[in++] = '*'; break;
	case mod: post[in++] = '%'; break;
	case unary: post[in++] = '#'; break;
	case eos: post[in] = '\0'; break;
	}


}

precedence getToken(char* symbol, int* n, char* expr) {
	/* get the next token, symbol is the character representation, which is returned, the
	token is represented by its enumerated value, which is returned in the function name */
	*symbol = expr[(*n)++];
	switch (*symbol) {
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '/': return divide;
	case '*': return times;
	case '%': return mod;
	case '#': return unary;
	case '\0': return eos;
	default: return operand; /* no error checking, default is operand */
	}
}

void postfix(char* expr, struct stack_* pt, char* post)
{ /* output the postfix of the expression. The expression string, stack, and the top are global */
	int n = 0;
	int in = 0;
	precedence token;
	char symbol;
	pt->stack[0] = eos;
	/*ispand icp arrays index is value of precedence
		lparen, rparen, plus, minus, times, divide, mod, eos*/
	int isp[] = { 0, 19, 12, 12, 13, 13, 13,14, 0 };
	int icp[] = { 20, 19, 12, 12, 13, 13, 13, 14,0 };

	for (token = getToken(&symbol, &n, expr); token != eos; token = getToken(&symbol, &n, expr)) {
		if (token == operand) {
			post[in] = symbol;
			++in;
		}

		else if (token == rparen) {
			/* unstack tokens until left parenthesis */
			while (pt->stack[pt->top] != lparen) {
				saveToken((precedence)pop(pt), post, in);
				in += 1;
			}

			pop(pt);/* discard the left parenthesis */
		}
		else {
			/* remove and print symbols whose isp is greater
			than or equal to the current token’s icp */
			while (isp[pt->stack[pt->top]] >= icp[token]) {
				saveToken((precedence)pop(pt), post, in);
				in += 1;
			}
			push(token, pt);
		}

	}
	while ((token = (precedence)pop(pt)) != eos) {
		saveToken(token, post, in);
		in += 1;
	}


}

int eval(struct stack_* pt, char* post) {

	precedence token;
	char symbol;
	int op1, op2;
	int n = 0;
	int length = strlen(post);
	token = getToken(&symbol, &n, post);
	if(length>2){
	while (token != eos) {
		if (token == operand)
			push((precedence)(symbol - '0'), pt);
		else {
			op2 = pop(pt);
			op1 = pop(pt);
			switch (token) {
			case plus: push((precedence)(op1 + op2), pt); break;
			case minus: push((precedence)(op1 - op2), pt); break;
			case times: push((precedence)(op1 * op2), pt); break;
			case divide: push((precedence)(op1 / op2), pt); break;
			case mod: push((precedence)(op1 % op2), pt); break;
			case unary: push((precedence)op1, pt); push((precedence)(-op2), pt); break;

			}
		}
		token = getToken(&symbol, &n, post);

	  }
	}
	else {
		push((precedence)(symbol - '0'),pt);
		/*We only consider the unary operator '- '*/
		op2= pop(pt);
		push((precedence)(-op2), pt);
	}

	return pop(pt);

}
void unarycheck(char* expr) {

	if (expr[0] == '-') expr[0] = '#';
	for(int i=0;expr[i]!='\0';i++)
	{
		if (expr[i] == '-' && expr[i - 1] == '(') expr[i] = '#';
	}
		
}
int main() {
	struct stack_* pt = newStack();
	char expr[MAX_EXPR_SIZE];
	char post[MAX_EXPR_SIZE] = { '0', };
	printf("Input: ");
	fgets(expr, sizeof(expr), stdin);
	strtok(expr, "\n");
	unarycheck(expr);
	postfix(expr, pt, post);
	printf("Prefix:");
	printf("%s\n", post);

	int result = eval(pt, post);
	printf("Result:%d", result);

	return 0;
}