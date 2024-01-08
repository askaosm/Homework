#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#define NUM 100
#define NAMELINE 30

int str_cmp(char* p1, char* p2);
char* str_copy(char* dst, const char* src);
void sort_name(char lastname[][NAMELINE], char firstname[][NAMELINE], int n);

int main(void) {

    FILE* ifp;

    int n, i;
    char lastname[NUM][NAMELINE] = { 0 };
    char firstname[NUM][NAMELINE] = { 0 };

    ifp = fopen("student.txt", "r");
    /*get n(=number of the student) from the file*/
    fscanf(ifp, "%d", &n);

    /*get the name and assign it int0 array*/
    for (i = 0; i < n; i++) {
        fscanf(ifp, "%s", lastname[i]);
        fscanf(ifp, "%s", firstname[i]);

    }
    fclose(ifp);

    
    /*Sort the name, following the lexical order rule*/
    sort_name(lastname, firstname, n);
   
    for (i = 0; i < n; i++)
        printf( "%s %s\n", lastname[i], firstname[i]);
    return 0;


}

int str_cmp(char* p1, char* p2) {
    for (; *p1 == *p2; p1++, p2++) {
        if (*p1 == '\0')return 0;
    }

    return (*p1 - *p2);
}

char* str_copy(char* dst, const char* src) {

    int i;

    
    i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';


    return dst;
}

void sort_name(char lastname[][NAMELINE], char firstname[][NAMELINE], int n) {
    int i, j;
    char temp1[NAMELINE]={0};
    char temp2[NAMELINE]={0};
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (str_cmp(lastname[i], lastname[j]) > 0) {
                str_copy(temp1, lastname[i]);
                str_copy(lastname[i], lastname[j]);
                str_copy(lastname[j], temp1);

                str_copy(temp2, firstname[i]);
                str_copy(firstname[i], firstname[j]);
                str_copy(firstname[j], temp2);

            }
            if (str_cmp(lastname[i], lastname[j]) == 0) {
                if (str_cmp(firstname[i], firstname[j]) > 0) {
                    str_copy(temp1, lastname[i]);
                    str_copy(lastname[i], lastname[j]);
                    str_copy(lastname[j], temp1);

                    str_copy(temp2, firstname[i]);
                    str_copy(firstname[i], firstname[j]);
                    str_copy(firstname[j], temp2);

                }
            }
        }
    }
}