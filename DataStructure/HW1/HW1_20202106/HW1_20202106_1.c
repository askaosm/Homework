#include <stdio.h>
#define NUM 10+1

char num[NUM] = { 0 };
    char result[NUM * 2 -1] = { 0 };
    int n;
void sort_slash(int j, int k);

int main(void) {

    
    fputs("input:",stdout);
    fgets(num, sizeof(num), stdin);
    fputs("output:\n", stdout);


    for (int i = 0; num[i] != '\0'; i++)
    {
        n++;

    }
    n = n - 1;   /*num[n]*/

    sort_slash(0, 0);
    return 0;
}


void sort_slash(int j, int k) {
    if (j == n-1) {
       
        result[k] = num[n-1];
        result[ k + 1] = '\0';
        
        return printf("%s\n", result);

    }
    result[k] = num[j];
    sort_slash(j+1, k + 1);
    result[k +1 ] = '/';
    sort_slash(j+1, k +2);
}


