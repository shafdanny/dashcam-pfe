#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


//gcc processor_encryption.c -o test

typedef struct{
    char* buf;
    int length;
} Data;


unsigned char* generate_random_key(int length);
int* convert_to_integer(char* buff,int n);
unsigned char* convert_to_char(int* buff,int n);
Data read(char* path);
void test();
void elapsedtime(struct timeval start,struct timeval end);