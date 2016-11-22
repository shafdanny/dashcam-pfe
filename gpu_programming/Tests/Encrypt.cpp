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

int main()
{
  unsigned char *buffer;
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
  for(int i = 0; i<data.length; i++)
    printf("%x ", data.buffer[i]); // prints a series of bytes

  fclose(fp);

  fp = fopen("image_rewrite.jpg", "wb");
  fwrite(data.buffer, sizeof(data.buffer[0]), data.length/sizeof(data.buffer[0]), fp);
  fclose(fp);
  printf("\n\n");
  // Construct kernel
  auto k = compile(xorFunction);

  // Set the number of QPUs to use
  k.setNumQPUs(4);

  // Allocate and initialise arrays shared between ARM and GPU
  SharedArray<int> a(16), b(16), r(16);
  srand(0);

  char message[] = "This is a messa";

  for (int i = 0; i < 16; i++) {
    a[i] = message[i];
    //b[i] = i+80;
  }

  b[0] = 17;
  b[1] = 19;
  b[2] = 17;
  b[3] = 23;
  b[4] = 17;
  b[5] = 19;
  b[6] = 17;
  b[7] = 31;
  b[8] = 17;
  b[9] = 19;
  b[10] = 17;
  b[11] = 23;
  b[12] = 17;
  b[13] = 19;
  b[14] = 17;
  b[15] = 15;

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
