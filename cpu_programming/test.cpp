#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "trivium.h"
// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

int main(void) {
    int i;
    printf("===== STARTING TEST ===== \n");
    unsigned int* key = (unsigned int *)calloc(3,sizeof(unsigned int));
	unsigned int* init = (unsigned int *)calloc(3,sizeof(unsigned int));
	ofstream myfile;

    myfile.open ("triviumUnsigned.key");
	
    key[0] = 0x8F;//random_integer();
	key[1] = 0;//random_integer();
	key[2] = 0;//random_integer()
	
	init[0] = 0;//random_integer();
	init[1] = 0;//random_integer();
	init[2] = 0;//random_integer();

    unsigned int *res = generate_random_key(10000, key, init);
    
    for(i=0;i<10000;i++){
		//printf("random int %d\n",res[i]);
        myfile << res[i];
	}

	int test = 8;
	printf("test %x\n",test >> 3);

    myfile.close();
	return 0;
}