#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "trivium.h"
#include "GPUEncryption.h"

using namespace std;

#define BLOCK_SIZE 64

// g++ -o TriviumKeygen TriviumKeygen.cpp BlockingQueue.h trivium.cpp GPUEncryption.cpp -std=c++11 -pthread


void copy_stdin2stdout(Trivium trivium, BlockingQueue<Data> *queue)
{
    
	int i = 0;
    for(;;) {
		unsigned char buffer[BLOCK_SIZE];
        unsigned char decrypted[BLOCK_SIZE];
		Data data;
		data.msg = (unsigned char*) malloc(BLOCK_SIZE);
		data.key = (unsigned char*) malloc(BLOCK_SIZE);
		
        size_t bytes = fread(data.msg,  sizeof(char),BLOCK_SIZE,stdin);
		
		data.len = bytes;
		data.id = i;
		
		i++;
		
	    unsigned char* result = trivium.generateKey(bytes);
		
		data.key = result;
		
        for(int i=0;i<bytes;i++){
            
        }
        if (bytes < BLOCK_SIZE)
            if (feof(stdin))
                break;
    }
	while(true) {}
}

int main(void) {
    BlockingQueue<Data> queue;
	
    GPUEncryption gpu = GPUEncryption(&queue);
    gpu.start();
	
	Trivium trivium;
	trivium.initiation();

	copy_stdin2stdout(trivium, &queue);
	
	sleep(20);
	
	return 0;
}
