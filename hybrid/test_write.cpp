#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;


// g++ -o test_write test_write.cpp
int main(){
    
    char* test1 = "putainnn";
    char* test2 = "connardd";
    char* test3 = "de merde";
    
    for(int i=0;i<5;i++){
        fwrite(test1,1,7,stdout);
        fflush(stoudt);
        fwrite(test2,1,7,stdout);
        fflush(stoudt);
        fwrite(test3,1,7,stdout);
        fflush(stoudt);
    }
}