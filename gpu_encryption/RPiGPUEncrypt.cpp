#include <stdio.h>
#include <stdlib.h>
#include "QPULib.h"
#include <sys/time.h>
#include <unistd.h>

#define LEN(n) ((n%4 == 0)? n/4 : n/4+1)
#define NUM_QPU 1

/** Simple structure to store a buffer of binary data **/
typedef struct  {
  long length;
  unsigned char *buffer;
} Data;

int* convert_to_integer(unsigned char* buff,int n)
{
	int i;
	int* tab;
	int cmp;
	int quarter = n/4;
	int len = LEN(n);
	int remaining;
	tab = (int*)malloc(len*sizeof(int));
	for(i=0,cmp=0;i<quarter;i++,cmp+=4){
		tab[i] = 0;
		tab[i] |= (int)(buff[cmp] << 24);
		tab[i] |= (int)(buff[cmp+1] << 16);
		tab[i] |= (int)(buff[cmp+2] << 8);
		tab[i] |= (int)(buff[cmp+3]);
		//printf("iteration value: %08X\n",tab[i]);
	}
	remaining = n%4;
	tab[quarter] = 0;
	for(i=0;i<remaining;i++){
		tab[quarter]|= (int)(buff[cmp+i] << (3-i)*8);
	}
	return tab;
}

//convert an int array to char* using each byte of the int array, CARE n IS THE SIZE OF THE DESIRED CHAR ARRAY not the INTEGER
unsigned char* convert_to_char(int* buff,int n)
{
	int i;
	int cmp = 0;
	unsigned char* tab;
	int quarter = n/4;
	int remaining = n%4;
	tab = (unsigned char*)malloc(n*sizeof(char));
	for(i=0,cmp=0;i<quarter;i++,cmp+=4){
		tab[cmp] = (unsigned char)(buff[i] >> 24) & 0xFF;
		tab[cmp+1] = (unsigned char)(buff[i] >> 16) & 0xFF;
		tab[cmp+2] = (unsigned char)(buff[i] >> 8) & 0xFF;
		tab[cmp+3] = (unsigned char)buff[i] & 0xFF;
		//printf("iteration value: %08X\n",tab[i]);
	}
	for(i=0;i<remaining;i++){
		tab[cmp+i] = (unsigned char)(buff[quarter] >> (3-i)*8) & 0xFF;
	}
	return tab;
}

/**
 * Function that executes in parallel using QPUs
 * Version 2: Multiple GPU with gather, receive and store
 */
void xorFunction2(Ptr<Int> p, Ptr<Int> q, Ptr<Int> r, Int n)
{
  Int inc = numQPUs() << 4;

  Ptr<Int> a = p + index() + (me() << 4);
  Ptr<Int> b = q + index() + (me() << 4);
  Ptr<Int> c = r + index() + (me() << 4);
  gather(a);gather(b);gather(c);
  
  Int pOld, qOld, rOld;

  For(Int i = 0, i<n, i=i+inc)
    gather(a+inc); gather(b+inc); gather(c+inc);
    receive(pOld); receive(qOld); receive(rOld);
    rOld = pOld^qOld;
    store(rOld, c);
    
    a = a+inc; b=b+inc; c=c+inc;
  End

  receive(pOld); receive(qOld); receive(rOld);
}

unsigned char* generate_random_key(int length)
{
	int i;
	unsigned char* result = (unsigned char*)malloc(length*sizeof(unsigned char));
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

char* encrypt_file(char* file_path, char* destination_path) {
  unsigned char *buffer;
  unsigned char *generated_key;

  FILE *fp;
  int filelen;

  Data data;

  srand(0);
  fp = fopen(file_path,"rb");  // r for read, b for binary
  fseek(fp, 0, SEEK_END);
  filelen = ftell(fp);
  rewind(fp);

  data.buffer = (unsigned char *)malloc((filelen+1)*sizeof(unsigned char));
  data.length = filelen;
  fread(data.buffer,data.length,1,fp); // read all bytes to our buffer
  fclose(fp);

  auto k = compile(xorFunction2);
  k.setNumQPUs(NUM_QPU);

  generated_key = generate_random_key(filelen);

  // Allocate and initialise arrays shared between ARM and GPU
  SharedArray<int> message(filelen), key(filelen), encrypted(filelen);

  for(int i=0;i<filelen;i++) {
    message[i] = data.buffer[i];
    key[i] = generated_key[i];
  }

  struct timeval stop, start,diff;
  
  gettimeofday(&start, NULL);
  k(&message, &key, &encrypted, filelen);
  gettimeofday(&stop, NULL);
  timersub(&stop, &start, &diff);

  printf("\n====== ENCRYPTION V1 STARTED====== \n");
  printf("Filelen %d \n", filelen);

  printf("\n\nMessage to be encrypted:\n");
  for(int i=0;i<30;i++) printf("%x ", message[i]);
  printf("\n\nGenerated key \n");
  for(int i=0;i<30;i++) printf("%x ", key[i]);
  printf("\n\nMessage after encryption:\n");
  for(int i=0;i<30;i++) printf("%x ", encrypted[i]);

  unsigned char *outputBuffer = (unsigned char *)malloc((filelen+1)*sizeof(unsigned char));
  for(int i=0;i<filelen;i++) outputBuffer[i] = encrypted[i];
  
  fp = fopen(destination_path, "wb");
  fwrite(outputBuffer, sizeof(outputBuffer[0]), data.length/sizeof(outputBuffer[0]), fp);
  fclose(fp);
  printf("\n\nGPU V1: Time taken: %ld.%06lds\n", diff.tv_sec, diff.tv_usec);

  printf("\n====== ENCRYPTION V1 ENDED ====== \n");

  int* integer_msg = convert_to_integer(data.buffer,filelen);
  int* integer_key = convert_to_integer(generated_key,filelen);
  int size = LEN(filelen);
  SharedArray<int> message2(size), key2(size), encrypted2(size);

  for(int i=0;i<size;i++) {
    message2[i] = integer_msg[i];
    key2[i] = integer_key[i];
  }

  gettimeofday(&start, NULL);
  // Invoke the kernel and display the result
  k(&message2, &key2, &encrypted2, size);
  gettimeofday(&stop, NULL);
  timersub(&stop, &start, &diff);
  printf("\n\nGPU V2: Time taken: %ld.%06lds\n", diff.tv_sec, diff.tv_usec);

  printf("\n\n");
}
