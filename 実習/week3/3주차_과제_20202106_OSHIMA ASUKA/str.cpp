#include "Str.h"
#include <cstdlib>
#include<cstring>


Str::Str(int leng){ //constructor
         
     len=leng; 
}

Str::Str(const char *neyong){
     str = new char[len+1]; //len를 크기로한 문자열의 메모리 할당   
    int i;
    for(i=0;neyong[i]!='\0'; i++)
        str[i] = neyong[i]; //str문자열을 초기화
     str[i]='\0';   
}

Str::~Str(){//destructor
  
     len=0;
     delete[] str;

}
int Str::length(){
    
    return len;  //문자열의 길이를 리턴
}
char* Str::contents(void){
    return str;  //문자열의 내용을 리턴
}

int Str::compare(class Str& a){
  
  return  strcmp(str, a.contents()); //내용 비교
}

int Str::compare(const char* a){

    return strcmp(str, a);   //str와 a문자열을 비교 
}

void Str::operator= (const char* a){
    int i;                       //str문자열에 a문자열을 대입
      for(  i=0; a[i]!='\0'; i++)
          str[i]= a[i];
      str[i]='\0';

}

void Str::operator= (class Str& a){

     str= a.contents(); //a내용은 대입한다
}
