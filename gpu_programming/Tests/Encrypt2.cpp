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
int main()
{
  unsigned char* buffer;
  unsigned char* key;
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

  fclose(fp);

  fp = fopen("image_rewrite.jpg", "wb");
  fwrite(data.buffer, sizeof(data.buffer[0]), data.length/sizeof(data.buffer[0]), fp);
  fclose(fp);
  printf("\n\n");
  // Construct kernel
  auto k = compile(xorFunction);

  // Set the number of QPUs to use
  k.setNumQPUs(1);
  key = generate_random_key(filelen);
  // Allocate and initialise arrays shared between ARM and GPU
  SharedArray<int> a(filelen), b(filelen), r(filelen);
  int i;
  
  for(i=0;i<filelen;i++){
	  a[i] |= data.buffer[i];
	  b[i] = key[i];
  }
  srand(0);
	
 

  clock_t t1=clock();
  // Invoke the kernel and display the result
  k(&a, &b, &r);
  clock_t t2=clock();
  double time = (double)(t2 - t1) / CLOCKS_PER_SEC;
  printf("The time taken is.. %f \n\n", time);

  printf("Message to be encrypted: %s \n", message);

  printf("====== ENCRYPTION ====== \n");
  for (int i = 0; i < 16; i++){
    printf("(%i, %i) = %i\n", a[i], b[i], r[i]);
  }

  for (int i = 0; i < 16; i++){
    message[i] = r[i];
  }
  printf("Message after encryption: %s \n", message);

  k(&r, &b, &a);
  printf("====== DECRYPTION ====== \n");
  for (int i = 0; i < 16; i++){
    printf("(%i, %i) = %i\n", r[i], b[i], a[i]);
  }

  for (int i = 0; i < 16; i++){
    message[i] = a[i];
  }
  printf("Message after decryption: %s \n", message);

  return 0;
}
