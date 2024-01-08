#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#define MAX 100
#define TRUE 1
#define FALSE 0

void sort(int* number, int n) {
	int i, j, temp;
	int min;

	for (i = 0; i < n - 1; i++) {
		min = i;
		for (j = i + 1; j < n; j++) {
			if (number[min] > number[j])  min = j;
		}

		temp = number[min];
		number[min] = number[i];
		number[i] = temp;
				
		
	}

}

int chech_arry(int* number, int n) {

	int work, i;
	work = number[0];
	for (i = 0; i < n; i++) {
		if (number[i] == work) {  /*n times running*/
			work += 1;
		}
		else break;
	}
	if (i == n) return TRUE;
	else return FALSE;

}


int main() {
	int n,i;
	int number[MAX];


    fputs("input\n", stdout);
	/*input the number of inputing number*/
	scanf("%d", &n);
	/*input the element of the string*/

	for (i = 0; i < n; i++) {
		scanf("%d", &number[i]);
	}


	/*sort fromt smallest number*/
	sort(number, n);

	/*chech arry function*/
	fputs("result\n",stdout);
	printf("%d", chech_arry(number, n));

	return 0;

}


