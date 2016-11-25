#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

#define LEN(n) ((n%4 == 0)? n/4 : n/4+1)

/**
 * Function that read a file and return its byte representation
 */
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

/**
* Function that generate a random key of size len using std rand
*/
unsigned char* generate_random_key(int length)
{
	int i;
	unsigned char* result = (char*)malloc(length*sizeof(char));
	if(result == NULL){
		printf("Out of memory generating key\n");
        exit(-1);
	}
	for (i = 0; i < length; i++)
	{
		result[i] = (char)rand()%256;
		//printf("rand: %x\n",result[i]);
	}
	return result;
}

/**
* Calculate and print the elapsedtime
*/
void elapsedtime(struct timeval start,struct timeval stop)
{
	struct timeval diff;
	timersub(&stop, &start, &diff);
	printf("time taken %ld.%06lds\n", diff.tv_sec, diff.tv_usec);
}

/**
* Function that convert a char* in to a int* using all space available
* n is the size of the char*
*/
int* convert_to_integer(char* buff,int n)
{
	int i;
	int* tab;
	int cmp;
	int len = LEN(n);
	int quarter = n/4;
	int remaining;
	tab = (int*)malloc(len*sizeof(int));
	for(i=0,cmp=0;i<quarter;i++,cmp+=4){
		tab[i] = 0;
		tab[i] |= (buff[cmp] << 24);
		tab[i] |= (buff[cmp+1] << 16);
		tab[i] |= (buff[cmp+2] << 8);
		tab[i] |= (buff[cmp+3]);
		//printf("iteration value: %08X\n",tab[i]);
	}
	remaining = n%4;
	tab[quarter] = 0;
	for(i=0;i<remaining;i++){
		tab[quarter]|= (buff[cmp+i] << (3-i)*8);
	}
	return tab;
}

/**
* Function that convert a int* in to a char*
* n is the size of the desired char*
*/
unsigned char* convert_to_char(int* buff,int n)
{
	int i;
	int cmp = 0;
	unsigned char* tab;
	int quarter = n/4;
	int remaining = n%4;
	tab = (unsigned char*)malloc(n*sizeof(char));
	for(i=0,cmp=0;i<quarter;i++,cmp+=4){
		tab[cmp] = (buff[i] >> 24) & 0xFF;
		tab[cmp+1] = (buff[i] >> 16) & 0xFF;
		tab[cmp+2] = (buff[i] >> 8) & 0xFF;
		tab[cmp+3] = buff[i] & 0xFF;
		//printf("iteration value: %08X\n",tab[i]);
	}
	for(i=0;i<remaining;i++){
		tab[cmp+i] = (buff[quarter] >> (3-i)*8) & 0xFF;
	}
	return tab;
}

/**
* Series of test using the previous function
*/
void test()
{
	printf("--------------------------test--------------------------\n");
	int i;
	char test[10]="coucou pd";
	unsigned char* key_test = generate_random_key(10);//"coupon"
	unsigned char result_test[10];
	char reconstruct_test[10];
	
	int* tab = convert_to_integer(test,10);
	int* int_key = convert_to_integer(key_test,10);
	unsigned char* decrypt_char;
	int result_int[10];
	int decrypt_int[10];
	int len = LEN(10);
	
	printf("original char array: ");
	for(i=0; i<10; ++i){
		printf(" %02X",test[i]);
	}
	printf("\n");
	printf("int array: ");
	for(i=0; i<len; ++i){
		printf(" %08X",tab[i]);
	}
	printf("\n\n");
	
	printf("encryption on integer\nres: ");
	for(i=0; i<len; ++i){
        result_int[i] = tab[i] ^ int_key[i];
		printf(" %08X",result_int[i]);
	}
	printf("\ndecryption on integer\nres: ");
	for(i=0; i<len; ++i){
        decrypt_int[i] = result_int[i] ^ int_key[i];
		printf(" %08X",decrypt_int[i]);
	}
	printf("\n");
	decrypt_char = convert_to_char(decrypt_int,10);
	printf("decrypted char array: ");
	for(i=0; i<10; ++i){
		printf(" %02X",decrypt_char[i]);
	}
	printf("\n\n");
	
	printf("mot: ");
	for(i=0; i<10; ++i){
		printf(" %02X",test[i]);
	}
	printf("\ncle: ");
	for(i=0; i<10; ++i){
		printf(" %02X",key_test[i]);
	}
	printf("\nres: ");
	for(i=0; i<10; ++i){
        result_test[i] = (char)(test[i] ^ key_test[i]);
		printf(" %02X",result_test[i]);
	}
	printf("\n\nrec: ");
	for(i=0; i<10; ++i){
        reconstruct_test[i] = (char)(result_test[i] ^ key_test[i]);
		printf(" %02X",reconstruct_test[i]);
	}
	printf("\n--------------------------end--------------------------\n");
}