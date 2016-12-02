#include <stdlib.h>
#include "QPULib.h"
#include <time.h>
#include <sys/time.h>

#define LEN(n) ((n%4 == 0)? n/4 : n/4+1)

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
  gather(a);gather(b);
  
  Int pOld, qOld, rOld;

  For(Int i = 0, i<n-1, i=i+inc)
    gather(a+inc); gather(b+inc);
    receive(pOld); receive(qOld);
    
    store(pOld^qOld, c);
    
    a = a+inc; b=b+inc; c=c+inc;
  End
  receive(pOld); receive(qOld);
  store(pOld^qOld, c);
}
//take a array of char and store it in to an array of integer using all space, n is the size of the char array
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

  fp = fopen("bigger.jpg","rb");  // r for read, b for binary
  printf("coucou\n");
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
  printf("\n\n");
  // Construct kernel
  auto k = compile(xorFunction2);

  // Set the number of QPUs to use
  k.setNumQPUs(1);
  int size = LEN(filelen);
  printf("original: %d size: %d\n",filelen,size);
  // Allocate and initialise arrays shared between ARM and GPU
  SharedArray<int> message(size), key(size), encrypted(size);
  //srand(0);

  int* integer_msg = convert_to_integer(data.buffer,filelen);
  int* integer_key = convert_to_integer(generated_key,filelen);
  
  for(int i=0;i<size;i++) {
    message[i] = integer_msg[i];
    key[i] = integer_key[i];
  }
  struct timeval stop, start,diff;
  gettimeofday(&start, NULL);
  // Invoke the kernel and display the result
  k(&message, &key, &encrypted, size);
  gettimeofday(&stop, NULL);
  timersub(&stop, &start, &diff);
  printf("GPU V1: %ld.%06lds\n", diff.tv_sec, diff.tv_usec);
  
  printf("Message to be encrypted:\n");
  for(int i=0;i<5;i++) printf("%x ", integer_msg[i]);
  printf("\n");
  for(int i=0;i<5;i++) printf("%x ", message[i]);

  printf("\n\nGenerated key \n");
  for(int i=0;i<5;i++) printf("%x ", key[i]);

  printf("\n====== ENCRYPTION ====== \n");
  printf("Message after encryption:\n");
  for(int i=0;i<5;i++) printf("%x ", encrypted[i]);
  
  
  //unsigned char *outputBuffer = (unsigned char *)malloc((filelen+1)*sizeof(unsigned char));
  //for(int i=0;i<filelen;i++) outputBuffer[i] = encrypted[i];
  /*
  int* tmp = (int*)malloc(size*sizeof(int));
  for(int i=0;i<size;i++){
	  tmp[i]=encrypted[i];
  }
  unsigned char *outputBuffer = convert_to_char(tmp,filelen);
  */
  /*
  fp = fopen("image_encrypted.jpg", "wb");
  fwrite(outputBuffer, sizeof(outputBuffer[0]), data.length/sizeof(outputBuffer[0]), fp);
  fclose(fp);
  printf("\n\n");
  */
  printf("\n====== DECRYPTION ====== \n");
  for(int i=0;i<5;i++) printf("%x ", message[i]);
  gettimeofday(&start, NULL);
  k(&encrypted, &key, &message, size);
  gettimeofday(&stop, NULL);
	timersub(&stop, &start, &diff);
	printf("\n%ld.%06lds\n", diff.tv_sec, diff.tv_usec);
  printf("Message after decryption:\n");
  for(int i=0;i<5;i++) printf("%x ", message[i]);
  printf("\nencrypted\n");
  for(int i=0;i<5;i++) printf("%x ", encrypted[i]);
  printf("\nkey\n");
  for(int i=0;i<5;i++) printf("%x ", key[i]);
  
  //unsigned char *decryptedBuffer = (unsigned char *)malloc((filelen+1)*sizeof(unsigned char));
  //for(int i=0;i<filelen;i++) decryptedBuffer[i] = message[i];
  
  int* tmp = (int*)malloc(size*sizeof(int));
  for(int i=0;i<size;i++){
	  tmp[i]=message[i];
  }
  printf("before copying to file\n");
  for(int i=0;i<5;i++) printf("%x ", tmp[i]);
  unsigned char *decryptedBuffer = convert_to_char(tmp,filelen);
  
  fp = fopen("image_decrypted_test.jpg", "wb");
  fwrite(decryptedBuffer, sizeof(decryptedBuffer[0]), data.length/sizeof(decryptedBuffer[0]), fp);
  fclose(fp);

  printf("\n\n");
  return 0;
}
