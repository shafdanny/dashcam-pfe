#include <stdlib.h>
#include "QPULib.h"
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define NUM_QPU 1

int count = 0;

/** Simple structure to store a buffer of binary data **/
typedef struct  {
  long length;
  unsigned char *buffer;
} Data;

/**
 * Function that executes in parallel using QPUs
 * There is a limit of 16 element in a vector
 * Version 1: Multiple GPU without gather, receive and store
 */
void xorFunction(Ptr<Int> p, Ptr<Int> q, Ptr<Int> r, Int n)
{
	Int inc = numQPUs() << 4;
	Int index;
  For(Int i = 0, i<n, i=i+inc)
	  index = (me() << 4) + i;
    Int a = p[index];
    Int b = q[index];

    a = a^b;

    r[index] = a;
  End
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

int main()
{
  unsigned char *buffer;
  unsigned char *generated_key;

  FILE *fp;
  int filelen;

  Data data;

  fp = fopen("../../assets/images/test_image_small.jpg","rb");  // r for read, b for binary
  fseek(fp, 0, SEEK_END);
  filelen = ftell(fp);
  rewind(fp);

  data.buffer = (unsigned char *)malloc((filelen+1)*sizeof(unsigned char));
  data.length = filelen;
  fread(data.buffer,data.length,1,fp); // read all bytes to our buffer
  
  for(int i = 0; i<20; i++)
    printf("%x ", data.buffer[i]); // prints a series of bytes

  fclose(fp);

  generated_key = generate_random_key(data.length);

  fp = fopen("image_rewrite.jpg", "wb");
  fwrite(data.buffer, sizeof(data.buffer[0]), data.length/sizeof(data.buffer[0]), fp);
  fclose(fp);
  printf("\n\n");
  // Construct kernel
  auto k = compile(xorFunction);
  auto k2 = compile(xorFunction2);

  // Set the number of QPUs to use
  k.setNumQPUs(NUM_QPU);
  k2.setNumQPUs(NUM_QPU);

  // Allocate and initialise arrays shared between ARM and GPU
  SharedArray<int> message(filelen), key(filelen), encrypted(filelen);
  srand(0);
  printf("Filelen %d \n", filelen);
  for(int i=0;i<filelen;i++) {
    message[i] = data.buffer[i];
    key[i] = generated_key[i];
  }
  
  struct timeval stop, start,diff;
  gettimeofday(&start, NULL);
  // Invoke the kernel and display the result
  k(&message, &key, &encrypted, filelen);
  gettimeofday(&stop, NULL);
  timersub(&stop, &start, &diff);
  printf("GPU V1: %ld.%06lds\n", diff.tv_sec, diff.tv_usec);
  
  gettimeofday(&start, NULL);
  k2(&message, &key, &encrypted, filelen);
  gettimeofday(&stop, NULL);
  timersub(&stop, &start, &diff);
  printf("GPU V2: %ld.%06lds\n", diff.tv_sec, diff.tv_usec);
  
  printf("Message to be encrypted:\n");
  for(int i=0;i<30;i++) printf("%x ", message[i]);

  printf("\n\nGenerated key \n");
  for(int i=0;i<30;i++) printf("%x ", key[i]);

  printf("\n====== ENCRYPTION ====== \n");
  printf("Message after encryption:\n");
  for(int i=0;i<30;i++) printf("%x ", encrypted[i]);
  printf("\n\nGenerated key \n");
  for(int i=0;i<30;i++) printf("%x ", key[i]);
  
  unsigned char *outputBuffer = (unsigned char *)malloc((filelen+1)*sizeof(unsigned char));
  for(int i=0;i<filelen;i++) outputBuffer[i] = encrypted[i];
  
  fp = fopen("image_encrypted.jpg", "wb");
  fwrite(outputBuffer, sizeof(outputBuffer[0]), data.length/sizeof(outputBuffer[0]), fp);
  fclose(fp);
  printf("\n\n");

  printf("\n====== DECRYPTION ====== \n");
  gettimeofday(&start, NULL);
  k2(&encrypted, &key, &message, filelen);
  gettimeofday(&stop, NULL);
  timersub(&stop, &start, &diff);
  printf("GPU V2: %ld.%06lds\n", diff.tv_sec, diff.tv_usec);
  
  printf("Message decrypted:\n");
  for(int i=0;i<30;i++) printf("%x ", message[i]);

  outputBuffer = (unsigned char *)malloc((filelen+1)*sizeof(unsigned char));
  for(int i=0;i<filelen;i++) outputBuffer[i] = message[i];
  
  fp = fopen("image_decrypted.jpg", "wb");
  fwrite(outputBuffer, sizeof(outputBuffer[0]), data.length/sizeof(outputBuffer[0]), fp);
  fclose(fp);
  printf("\n\n");

}
  