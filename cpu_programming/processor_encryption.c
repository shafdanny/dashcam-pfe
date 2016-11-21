#include <stdio.h>
#include <stdlib.h>
#include <sys\timeb.h> 
#include <string.h>
#include <time.h>


//gcc processor_encryption.c -o test

typedef struct{
    char* buf;
    int length;
} Data;


void test();
void elapsedtime(struct timeb start,struct timeb end);
//read a file and return a Data containing a byte[] and its length
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

//generate a random key of size len
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
	srand ((unsigned int) time (NULL));
	printf("%d\n",rand()%3);
	Data file = read("test.jpg");
	int len = file.length;
	int i;
	struct timeb start, end;
	
	test();
	
	unsigned char* cypher = (char*)malloc(len*sizeof(char));
	unsigned char* uncypher = (char*)malloc(len*sizeof(char)+1);
	if(cypher == NULL || uncypher == NULL){
		printf("Out of memory result\n");
        exit(-1);
	}
	//genere la clé, un random de la meme taille que notre buffer
	unsigned char* key = generate_random_key(len);
	printf("chiffrement length: %d octets\n",len);//end of file at the end
	ftime(&start);
	//réalise le xor entre la clé et le buffer
	for(i=0; i<len; ++i){
        cypher[i] = (char)(file.buf[i] ^ key[i]);
	}
	ftime(&end);
	elapsedtime(start,end);
	printf("dechiffrement\n");
	ftime(&start);
	//réalise le déchiffrement
	for(i=0; i<len; ++i){
        uncypher[i] = (char)(cypher[i] ^ key[i]);
	}
	ftime(&end);
	if(strcmp(file.buf,uncypher) == 0)
		printf("youpi\n");
	elapsedtime(start,end);
	
	FILE * fichier;
	fichier = fopen("result.jpg" , "wb");
	fwrite(uncypher,sizeof(char),len,fichier);

}

void elapsedtime(struct timeb start,struct timeb end)
{
	int time_spend = (int) (1000.0 * (end.time - start.time)+ (end.millitm - start.millitm));
	printf("time elapsed: %d ms\n",time_spend);
}

void test()
{
	printf("--------------------------test--------------------------\n");
	int i;
	char test[10]="coucou";
	unsigned char* key_test = generate_random_key(10);//"coupon"
	unsigned char result_test[10];
	char reconstruct_test[10];
	
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