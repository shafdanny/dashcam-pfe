#include "trivium_multi_core.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;



//g++ -o crypt crypt.cpp trivium_multi_core.cpp trivium.cpp 



int main(){
    
    FILE *fp;
    fp = fopen("images/blossom.jpg","rb");  // r for read, b for binary
    fseek(fp, 0, SEEK_END);
    int filelen = ftell(fp);
    rewind(fp);
    unsigned char* buffer = (unsigned char *)malloc((filelen+1)*sizeof(char));
    fread(buffer,filelen,1,fp);
    fclose(fp);
    
    TriviumMultiCore trivium;
    trivium.initiation();
    unsigned char* result = trivium.cypher(buffer,filelen);
    
    cout << "end encryption" << endl;
    fp = fopen("image_crypted_test1.jpg", "wb");
    fwrite(result, sizeof(result[0]), filelen/sizeof(result[0]), fp);
    fclose(fp);
    
    
    fp = fopen("images/cat.jpg","rb");  // r for read, b for binary
    fseek(fp, 0, SEEK_END);
    filelen = ftell(fp);
    rewind(fp);
    buffer = (unsigned char *)malloc((filelen+1)*sizeof(char));
    fread(buffer,filelen,1,fp);
    fclose(fp);

    result = trivium.cypher(buffer,filelen);
    
    fp = fopen("image_crypted_test2.jpg", "wb");
    fwrite(result, sizeof(result[0]), filelen/sizeof(result[0]), fp);
    fclose(fp);
    
}