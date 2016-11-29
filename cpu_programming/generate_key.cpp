<<<<<<< HEAD
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
    char* buf;
    int length;
} Data;

//generate a random key of size len
unsigned char* generate_random_key(int length)
{
	int i;
	unsigned char* result = (unsigned char*)malloc(length*sizeof(char));
	if(result == NULL){
		printf("Out of memory generating key\n");
        exit(-1);
	}
	for (i = 0; i < length; i++)
	{
		result[i] = (char)rand()%256;
	}
	return result;
}

Data read(char* path)
{
	FILE *fileptr;
	char *buffer;
	long filelen;

	fileptr = fopen(path, "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file

	buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
	Data result;
	result.length = filelen;
	result.buf = (char *)malloc((filelen+1)*sizeof(char));
	if(result.buf == NULL){
		printf("Out of memory\n");
        exit(-1);
	}
	fread(result.buf, filelen, 1, fileptr); // Read in the entire file
	fclose(fileptr);
	return result;
}

int main()
{
    srand ((unsigned int) time (NULL));
    Data file = read("../gpu_programming/Tests/test_image.jpg");
	int len = file.length;
    
    unsigned char* key = generate_random_key(len);
    
    std::ofstream keyFile ("../gpu_programming/Tests/test_image.key", std::ios::out | std::ios::binary);
    keyFile.write(reinterpret_cast<const char*>(key), len);
    
    
    return 0;
=======
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
    char* buf;
    int length;
} Data;

//generate a random key of size len
unsigned char* generate_random_key(int length)
{
	int i;
	unsigned char* result = (unsigned char*)malloc(length*sizeof(char));
	if(result == NULL){
		printf("Out of memory generating key\n");
        exit(-1);
	}
	for (i = 0; i < length; i++)
	{
		result[i] = (char)rand()%256;
	}
	return result;
}

Data read(char* path)
{
	FILE *fileptr;
	char *buffer;
	long filelen;

	fileptr = fopen(path, "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file

	buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
	Data result;
	result.length = filelen;
	result.buf = (char *)malloc((filelen+1)*sizeof(char));
	if(result.buf == NULL){
		printf("Out of memory\n");
        exit(-1);
	}
	fread(result.buf, filelen, 1, fileptr); // Read in the entire file
	fclose(fileptr);
	return result;
}

int main()
{
    srand ((unsigned int) time (NULL));
    Data file = read("../gpu_programming/Tests/test_image.jpg");
	int len = file.length;
    
    unsigned char* key = generate_random_key(len);
    
    std::ofstream keyFile ("../gpu_programming/Tests/test_image.key", std::ios::out | std::ios::binary);
    keyFile.write(reinterpret_cast<const char*>(key), len);
    
    
    return 0;
>>>>>>> c2184a9bb76ec30e7904d3371e70543024a9ff74
}