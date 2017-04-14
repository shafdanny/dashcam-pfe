#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "trivium.h"
#include "GPUEncryption.h"

using namespace std;

#define BLOCK_SIZE 65536

// g++ -o TriviumKeygen TriviumKeygen.cpp BlockingQueue.h trivium.cpp GPUEncryption.cpp -std=c++11 -pthread


void copy_stdin2stdout(Trivium trivium, BlockingQueue<Data> *queue)
{
	//cout << "Starting copystdinstdout" << endl;
    
	int i = 0;
    for(;;) {
		//cout << "In loop stdinstdout" << endl;

		unsigned char buffer[BLOCK_SIZE];
		//cout << "Initialized buffer" << endl;


		Data data;
		data.msg = (unsigned char*) malloc(BLOCK_SIZE);
		data.key = (unsigned char*) malloc(BLOCK_SIZE);

		//cout << "Allocated data" << endl;

        size_t bytes = fread(data.msg,  sizeof(char),BLOCK_SIZE,stdin);
		//cout << "Fread done " << bytes << endl;

		
		data.len = bytes;
		data.id = i;
		
		i++;
		
	    unsigned char* result = trivium.generateKey(bytes);
		//cout << "Key generated" << bytes << endl;


		data.key = result;
		//data.msg = buffer;
		//cout << "Pushing data to queue" << endl;
		queue->push(data);
		
		//cout << "Key: " << data.key << endl;
		//cout << "Msg: " << data.msg << endl;
		//cout << "Len: " << data.len << endl;
		
        //fwrite(buffer, sizeof(char), bytes, stdout);
        //fflush(stdout);
		//sleep(5);
		
        if (bytes < BLOCK_SIZE)
            if (feof(stdin))
                break;
    }
	//while(true) {}
}

int main(void) {
    BlockingQueue<Data> queue;
	
	//cout << "Starting TriviumKeygen" << endl;

    GPUEncryption gpu = GPUEncryption(&queue);
    gpu.start();
	
	Trivium trivium;
	trivium.initiation();

	copy_stdin2stdout(trivium, &queue);
	
	sleep(1);
	
	return 0;
}
