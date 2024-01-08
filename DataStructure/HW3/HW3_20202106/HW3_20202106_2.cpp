/*ローカル変数に変える必要あり*/
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include<stdlib.h>
#define MAX_STACK_SIZE 100 
#define MAX_EXPR_SIZE 20+1 

typedef enum {
	lparen, rparen, plus, minus, times, divide, mod, eos, operand
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

void saveToken(precedence token, char* result,int in) {
	switch (token) {
	case lparen: result[in++] = '('; ; break;
	case rparen: result[in++] = ')'; break;
	case plus: result[in++] = '+'; break;
	case minus: result[in++] = '-'; break;
	case divide: result[in++] = '/'; break;
	case times: result[in++] = '*'; break;
	case mod: result[in++] = '%'; break;
	case eos: result[in] = '\0'; break;
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
	case '\0': return eos;
	default: return operand; /* no error checking, default is operand */
	}
}

void getpostfix(char* expr, struct stack_* pt, char* result)
{ /* output the postfix of the expression. The expression string, stack, and the top are global */
	int n = 0;
	int in = 0;
	precedence token;
	char symbol;
	pt->stack[0] = eos;
	/*ispand icp arrays index is value of precedence
		lparen, rparen, plus, minus, times, divide, mod, eos*/
	int isp[] = { 0, 19, 12, 12, 13, 13, 13, 0 };
	int icp[] = { 20, 19, 12, 12, 13, 13, 13, 0 };

	for (token = getToken(&symbol, &n, expr); token != eos; token = getToken(&symbol, &n, expr)) {
		if (token == operand) {
			result[in] = symbol;
			++in;
		}

		else if (token == rparen) {
			/* unstack tokens until left parenthesis */
			while (pt->stack[pt->top] != lparen) {
				saveToken((precedence)pop(pt), result, in);
				in += 1;
			}

			pop(pt);/* discard the left parenthesis */
		}
		else {
			/* remove and print symbols whose isp is greater
			than or equal to the current token’s icp */
			while (isp[pt->stack[pt->top]] >= icp[token]) {
				saveToken((precedence)pop(pt), result, in);
				in += 1;
			}
			push(token, pt);
		}

	}
	while ((token = (precedence)pop(pt)) != eos) {
		saveToken(token, result, in);
		in += 1;
	}


}

void reverse(char* expr) {

	int n = strlen(expr);
	char temp;
	for (int i = 0; i < n / 2; i++) {
		temp = expr[i];
		expr[i] = expr[n - 1 - i];
		expr[n - 1 - i] = temp;
	}

}
void change(char* exp) {
	int i = 0;
	while (exp[i] != '\0')
	{
		if (exp[i] == '(')
			exp[i] = ')';
		else if (exp[i] == ')')
			exp[i] = '(';
		i++;
	}
}

void prefix(char* expr, struct stack_* pt,char* result) {
	reverse(expr);
	change(expr);
	getpostfix(expr, pt, result);
	reverse(result);

}
int main() {
	struct stack_* pt = newStack();
	char expr[MAX_EXPR_SIZE];
	char result[MAX_EXPR_SIZE] = { '0', };
	printf("Infix: ");
	fgets(expr, sizeof(expr), stdin);
	prefix(expr, pt, result);
	printf("Prefix:");
	printf("%s", result);

	return 0;
}