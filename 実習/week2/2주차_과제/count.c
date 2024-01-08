#include "header.h"

void count (int N, int* pagenum){
    int n,temp;
    for(n=1;n<=N;n++){
         temp=n;
             while(temp>=10){//temp가 10보다 작을 때까지 진행함
              pagenum[temp%10]++; //일의 자리의 숫자를 카운트
              temp/=10; //나누기 전에 십의 자리였던 수를 일의 자리로 생각하고 다시 while 
             } 
              pagenum[temp%10]++; //원래 숫자n가 10보다 작은 경우 아니면 원래 숫자의 제일 큰자리였던 수를 카운트
    }
 }

