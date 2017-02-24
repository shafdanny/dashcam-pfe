#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "trivium.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

#define BLOCK_SIZE 2048

void copy_stdin2stdout(Trivium trivium)
{
    unsigned char buffer[BLOCK_SIZE];
    for(;;) {
        size_t bytes = fread(buffer,  sizeof(char),BLOCK_SIZE,stdin);
		
		//cout << "Original: " << buffer << endl;
		
	    unsigned char* result = trivium.cypher(buffer,bytes);
		
		//cout << "Result: " << result << endl;
		
        fwrite(result, sizeof(char), bytes, stdout);
        fflush(stdout);
        if (bytes < BLOCK_SIZE)
            if (feof(stdin))
                break;
    }

}

int main(void) {
	// Initialize trivium stuff
	Trivium trivium;
	trivium.initiation();
		
	copy_stdin2stdout(trivium);
	return 0;
}
