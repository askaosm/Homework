#include "header.h"

int main(){

    int T; //the number of test cases
    int N; //the number of the page
    scanf("%d",&T);

    for(int i=0; i<T; i++){
       int pagenum[10]={0,}; // initilize 0  
       scanf("%d", &N);  
       count(N,pagenum); //베이지를 세우고 pagenum에 넣다
       output(pagenum);  //count한 결과를 출력한다.
    } 
   
    return 0;

}


