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

void initiation(int** r1,int** r2,int** r3,int* key,int* init)
{
	int i;
	int len = 4*288;
	int t1,t2,t3;
	int bit1,bit2,bit3;
	(*r1)[0] = key[0];
	(*r1)[1] = key[1];
	(*r2)[0] = init[0];
	(*r2)[1] = init[1];
	
	printf("init: %x key: %x\n",init[0],key[0]);
	//on veut les 80 premiers bits de key et de init 64 + 16
	//les 16 premiers bits 
	(*r1)[2] = key[2] >> 16;
	(*r2)[2] = init[2] >> 16;
	//le reste a 0
	(*r1)[2] = (*r1)[2] << 16;
	(*r2)[2] = (*r2)[2] << 16;
	
	//les 3 derniers bits de r3 valent 1
	(*r3)[3] = 8;	
	
	for(i=0;i<len;i++){
		// 66 ou 91 & 92 ou 93 ou 171
		t1 = ((*r1)[2] >> 1) ^ ( ((*r1)[2] >> 26) & ((*r1)[2] >> 27) ) ^ ((*r1)[2] >> 28) ^ ((*r2)[2] >> 12);
		// 162 ou 175 et 176 ou 177 ou 264
		t2 = ((*r2)[2] >> 3) ^ ( ((*r2)[2] >> 16) & ((*r2)[2] >> 17) ) ^ ((*r2)[2] >> 18) ^ ((*r3)[2] >> 21);
		// 243 ou 286 et 287 ou 288 ou 69
		t3 = ((*r3)[2]) ^ ( ((*r3)[3] >> 15) & ((*r3)[3] >> 16) ) ^ ((*r3)[3] >> 17) ^ ((*r1)[2] >> 4);
		
		//on decale chacun des tableaux en recuperant le dernier bit du tableau précédant de chacun des tableaux [93,84,111]
		//tableau 1
		bit1 = ((*r1)[0] >> 31) & 1;
		bit2 = ((*r1)[1] >> 31) & 1;
		(*r1)[0] = (t1 & 1) | ((*r1)[0] << 1);
		(*r1)[1] = (bit1 & 1) | ((*r1)[1] << 1);
		(*r1)[2] = (bit2 & 1) | ((*r1)[2] << 1);
		
		//tableau 2
		bit1 = ((*r2)[0] >> 31) & 1;
		bit2 = ((*r2)[1] >> 31) & 1;
		(*r2)[0] = (t2 & 1) | ((*r2)[0] << 1);
		(*r2)[1] = (bit1 & 1) | ((*r2)[1] << 1);
		(*r2)[2] = (bit2 & 1) | ((*r2)[2] << 1);
		
		//tableau3
		bit1 = ((*r3)[0] >> 31) & 1;
		bit2 = ((*r3)[1] >> 31) & 1;
		bit3 = ((*r3)[2] >> 31) & 1;
		(*r3)[0] = (t3 & 1) | ((*r3)[0] << 1);
		(*r3)[1] = (bit1 & 1) | ((*r3)[1] << 1);
		(*r3)[2] = (bit2 & 1) | ((*r3)[2] << 1);
		(*r3)[3] = (bit3 & 1) | ((*r3)[3] << 1);
	}
}

unsigned int get_integer(int** r1,int** r2,int** r3)
{
	int i;
	int t1,t2,t3;
	int bit1,bit2,bit3;
	unsigned int z = 0;
	
	for(i=0;i<32;i++){
		//66:2 ou 93:29
		t1 = (((*r1)[2] >> 1) ^ ((*r1)[2] >> 28)) & 1;
		//162:68:4 177:83
		t2 = (((*r2)[2] >> 3) ^ ((*r2)[2] >> 18)) & 1;
		//243:65:1 288:110:14
		t3 = (((*r3)[2]) ^ ((*r3)[3] >> 13)) & 1;
		
		//compute the output bit
		z = z << 1;
		z |= t1 ^ t2 ^ t3;
		
		//compute the 3 input bit
		//91 et 92 ou 171
		t1 ^= (((*r1)[2] >> 26) & ((*r1)[2] >> 27)) ^ ((*r2)[2] >> 12);
		//175 et 176 ou 264
		t2 ^= (((*r2)[2] >> 16) & ((*r2)[2] >> 17)) ^ ((*r3)[2] >> 21);
		//286 et 287 ou 69
		t3 ^= (((*r3)[3] >> 11) & ((*r3)[3] >> 12)) ^ ((*r1)[2] >> 4);
		
		//rotate the registers
		//tableau1
		bit1 = ((*r1)[0] >> 31) & 1;
		bit2 = ((*r1)[1] >> 31) & 1;
		(*r1)[0] = (t1 & 1) | ((*r1)[0] << 1);
		(*r1)[1] = (bit1 & 1) | ((*r1)[1] << 1);
		(*r1)[2] = (bit2 & 1) | ((*r1)[2] << 1);
		
		//tableau 2
		bit1 = ((*r2)[0] >> 31) & 1;
		bit2 = ((*r2)[1] >> 31) & 1;
		(*r2)[0] = (t2 & 1) | ((*r2)[0] << 1);
		(*r2)[1] = (bit1 & 1) | ((*r2)[1] << 1);
		(*r2)[2] = (bit2 & 1) | ((*r2)[2] << 1);
		
		//tableau3
		bit1 = ((*r3)[0] >> 31) & 1;
		bit2 = ((*r3)[1] >> 31) & 1;
		bit3 = ((*r3)[2] >> 31) & 1;
		(*r3)[0] = (t3 & 1) | ((*r3)[0] << 1);
		(*r3)[1] = (bit1 & 1) | ((*r3)[1] << 1);
		(*r3)[2] = (bit2 & 1) | ((*r3)[2] << 1);
		(*r3)[3] = (bit3 & 1) | ((*r3)[3] << 1);
	}
	return z;
}

unsigned char* generate_random_bkey(int length)
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

/**
* Generate a pseudo random int[] using trivium
* key and init must have a minimum of 80 byte length, 3 integer
*/
unsigned int* generate_random_key(int len,int* key,int* init)
{
	int i;
	unsigned int* res = (unsigned int*)malloc(len*sizeof(int));
	int* regist1 = (int*)calloc(3,sizeof(int));
	int* regist2 = (int*)calloc(3,sizeof(int));
	int* regist3 = (int*)calloc(4,sizeof(int));
	printf("\ninit: %d key: %d\n",init[0],key[0]);
	initiation(&regist1,&regist2,&regist3,key,init);
	for(i=0;i<len;i++){
		res[i] = get_integer(&regist1,&regist2,&regist3);
	}
	free(regist1);
	free(regist2);
	free(regist3);
	return res;
}

int main()
{
  unsigned char *buffer;
  unsigned int *generated_key;
  unsigned char* m_key;

  FILE *fp;
  int filelen;

  Data data;

  fp = fopen("./images/ornito.jpg","rb");  // r for read, b for binary
  fseek(fp, 0, SEEK_END);
  filelen = ftell(fp);
  rewind(fp);

  data.buffer = (unsigned char *)malloc((filelen+1)*sizeof(unsigned char));
  data.length = filelen;
  fread(data.buffer,data.length,1,fp); // read all bytes to our buffer
  
  for(int i = 0; i<20; i++)
    printf("%x ", data.buffer[i]); // prints a series of bytes

  fclose(fp);

  int size = LEN(filelen);
  
  int* t_key = (int*)calloc(3,sizeof(int));
  int* init = (int*)calloc(3,sizeof(int));
	
	t_key[0] = 3861042005;//random_integer();
	t_key[1] = 1559349736;//random_integer();
	t_key[2] = 777729657;//random_integer()
	
	init[0] = 873716055;//random_integer();
	init[1] = 1559349736;//random_integer();
	init[2] = 3547487477;//random_integer();
	printf("start generating random key size: %d\n",size);
  generated_key = generate_random_key(size,t_key,init);
  m_key = generate_random_bkey(filelen);
  printf("end generation random key\n\n");
  // Construct kernel
  auto k = compile(xorFunction2);

  // Set the number of QPUs to use
  k.setNumQPUs(8);
  printf("original: %d size: %d\n",filelen,size);
  // Allocate and initialise arrays shared between ARM and GPU
  SharedArray<int> message(size), key(size), encrypted(size);
  //srand(0);

  int* integer_msg = convert_to_integer(data.buffer,filelen);
  int* integer_key = convert_to_integer(m_key,filelen);
  
  for(int i=0;i<size;i++) {
    message[i] = integer_msg[i];
    key[i] = generated_key[i];
  }
  struct timeval stop, start,diff;
  gettimeofday(&start, NULL);
  // Invoke the kernel and display the result
  k(&message, &key, &encrypted, size);
  printf("before crash\n");
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
