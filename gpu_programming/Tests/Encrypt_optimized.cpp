#include <stdlib.h>
#include "QPULib.h"
#include <time.h>



//make QPU=1 
/** Simple structure to store a buffer of binary data **/
typedef struct  {
  long length;
  unsigned char *buffer;
} Data;


Data read(char* path);
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
	unsigned char* result = (unsigned char*)malloc(length*sizeof(char));
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
	Data file = read("big.jpg");
	long filelen = file.length;
	printf("length %d\n",filelen);
	int i;
	clock_t t1, t2;
	unsigned char* key = generate_random_key(filelen);
	// Construct kernel
	auto k = compile(xorFunction);

	// Set the number of QPUs to use
	k.setNumQPUs(1);
	// Allocate and initialise arrays shared between ARM and GPU
	SharedArray<int> a(filelen), b(filelen), r(filelen);

	for(i=0;i<filelen;i++){
		a[i] = file.buffer[i];
		b[i] = key[i];
	}
	srand(0);



	t1=clock();
	// Invoke the kernel and display the result
	k(&a, &b, &r);
	t2=clock();
	double time = (double)(t2 - t1) / CLOCKS_PER_SEC;

	printf("Message to be encrypted: %d bytes \n",filelen);

	printf("====== ENCRYPTION ====== \n");
	printf("The time taken is.. %f\n\n", time);
	for (int i = 0; i < 2; i++){
		printf("(%i, %i) = %i\n", a[i], b[i], r[i]);
	}
	t1=clock();
	k(&r, &b, &a);
	t2=clock();
	printf("====== DECRYPTION ====== \n");
	time = (double)(t2 - t1) / CLOCKS_PER_SEC;
	printf("The time taken is.. %f \n\n", time);
	for (int i = 0; i < 2; i++){
		printf("(%i, %i) = %i\n", r[i], b[i], a[i]);
	}
	FILE * fichier;
	fichier = fopen("result.jpg" , "wb");
	fwrite(uncypher,sizeof(char),len,fichier);
	
	return 0;
}

Data read(char* path)
{
	FILE *fileptr;
	long filelen;

	fileptr = fopen(path, "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file

	Data result;
	result.length = filelen;
	result.buffer = (unsigned char *)malloc((filelen+1)*sizeof(char));
	if(result.buffer == NULL){
		printf("Out of memory\n");
        exit(-1);
	}
	fread(result.buffer, filelen, 1, fileptr); // Read in the entire file
	fclose(fileptr);
	return result;
}
