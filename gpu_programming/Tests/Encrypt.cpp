#include <stdlib.h>
#include "QPULib.h"
#include <time.h>

/** Simple structure to store a buffer of binary data **/
typedef struct  {
  long length;
  unsigned char *buffer;
} Data;

/** 
 * Function that executes in parallel using QPUs
 * There is a limit of 16 element in a vector
 */
void xorFunction(Ptr<Int> p, Ptr<Int> q, Ptr<Int> r)
{
  Int a = *p;
  Int b = *q;
  
  a = a^b;
  
  *r = a;
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
  long filelen;

  Data data;

  fp = fopen("test_image_small.jpg","rb");  // r for read, b for binary
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

  // Set the number of QPUs to use
  k.setNumQPUs(1);

  // Allocate and initialise arrays shared between ARM and GPU
  SharedArray<int> message(filelen), key(filelen), encrypted(filelen);
  srand(0);

  for(int i=0;i<filelen;i++) {
    message[i] = data.buffer[i];
    key[i] = generated_key[i];
  }

  clock_t t1=clock();
  // Invoke the kernel and display the result
  k(&message, &key, &encrypted);
  clock_t t2=clock();
  
  double time = (double)(t2 - t1) / CLOCKS_PER_SEC;
  printf("The time taken is.. %f \n\n", time);

  printf("Message to be encrypted:\n");
  for(int i=0;i<20;i++) printf("%x ", message[i]);

  printf("\n====== ENCRYPTION ====== \n");
  printf("Message after encryption:\n");
  for(int i=0;i<20;i++) printf("%x ", encrypted[i]);
  
  unsigned char *outputBuffer = (unsigned char *)malloc((filelen+1)*sizeof(unsigned char));
  for(int i=0;i<filelen;i++) outputBuffer[i] = encrypted[i];
  
  fp = fopen("image_encrypted.jpg", "wb");
  fwrite(outputBuffer, sizeof(outputBuffer[0]), data.length/sizeof(outputBuffer[0]), fp);
  fclose(fp);
  printf("\n\n");

  printf("\n====== DECRYPTION ====== \n");
  k(&encrypted, &key, &message);
  printf("Message after decryption:\n");
  for(int i=0;i<20;i++) printf("%x ", message[i]);

  return 0;
}
