#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#define LEN(n) ((n%4 == 0)? n/4 : (n/4)+1)
#define LENVECTOR(n) ((n%16 == 0)? (n/16) : (n/16)+1)

int count = 0;

/** Simple structure to store a buffer of binary data **/
typedef struct  {
  long length;
  unsigned char *buffer;
} Data;

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
		//printf("etat registre 1 %x\netat registre2 %x\n\n",(*r1)[0],(*r1)[1]);
		t1 = (((*r1)[2] >> 1) ^ ((*r1)[2] >> 28)) & 1;
		//162:68:4 177:83
		t2 = (((*r2)[2] >> 3) ^ ((*r2)[2] >> 18)) & 1;
		//243:65:1 288:110:14
		t3 = (((*r3)[2]) ^ ((*r3)[3] >> 13)) & 1;
		
		//printf("t1 %x t2 x t3 %x\n",t1,t2,t3);
		
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
		
		//printf("etat registre 1 %x\netat registre2 %x\n",(*r1)[0],(*r1)[1]);
	}
	return z;
}

unsigned int* trivium_cpu(int* message,int** r1,int** r2,int** r3,int n)
{
	int i,k;
    int number_of_vector = LENVECTOR(n);
    printf("len de merde %d %d\n",n,number_of_vector*16);
	unsigned int* res =(unsigned int*)calloc(number_of_vector*16,sizeof(int));
    unsigned int* vector = (unsigned int*)malloc(16*sizeof(int));
	for(i=0;i<number_of_vector;i++){
        for(k=0;k<16;k++){
            int key =  get_integer(r1,r2,r3);
            vector[15-k] = key;
        }
        for(k=0;k<16;k++){
            //printf("result inside cpu %x %x\n",vector[k],message[i*16+k]);
            res[i*16+k] = message[i*16+k] ^ vector[k];
        }
		//res[i] = message[i] ^ key;
        //res[i] = key;
	}
	return res;
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


int main()
{
    FILE *fp;
    Data data;

    fp = fopen("crypted_test.jpg","rb");  // r for read, b for binary
    fseek(fp, 0, SEEK_END);
    int filelen = ftell(fp);
    rewind(fp);
    
    int len = filelen;
	int nb_integer = LEN(len);
	int nb_vector = LENVECTOR(nb_integer);
    nb_integer = nb_vector * 16;

    data.buffer = (unsigned char *)malloc((filelen+1)*sizeof(unsigned char));
    //data.buffer = generate_random_bkey(nb_vector*64);
    data.length = len;
    fread(data.buffer,data.length,1,fp);
  
    fclose(fp);
  
  printf("size of file %d byte integer %d vector  %d right integer %d\n",len,nb_integer,nb_vector,LEN(len));
  
  int i;
  int* t_key = (int*)calloc(3,sizeof(int));
  int* init = (int*)calloc(3,sizeof(int));
  int* regist1 = (int*)calloc(3,sizeof(int));
  int* regist2 = (int*)calloc(3,sizeof(int));
  int* regist3 = (int*)calloc(4,sizeof(int));
	
	t_key[0] = 3861042005;//random_integer();
	t_key[1] = 1559349736;//random_integer();
	t_key[2] = 777729657;//random_integer()
	
	init[0] = 873716055;//random_integer();
	init[1] = 1559349736;//random_integer();
	init[2] = 3547487477;//random_integer();
	
	printf("start generating random key size\n");
	initiation(&regist1,&regist2,&regist3,t_key,init);
	int* integer_msg = convert_to_integer(data.buffer,nb_vector*64);
    
    
	printf("before\n");
	for(i=0;i<20;i++){
		//printf("regist3 %x\n",regist3[i]);
        printf("message to be encrypted %x\n",integer_msg[LEN(len)-19+i]);
	}
    /*
    for(i=0;i<4;i++){
		//printf("regist3 %x\n",regist3[i]);
        printf("message to be encrypted end %x\n",integer_msg[16+i]);
	}
    */
    unsigned int* cpu_encrypted = trivium_cpu(integer_msg,&regist1,&regist2,&regist3,nb_integer);
	for(i=0;i<4;i++){
		printf("result cpu %x\n",cpu_encrypted[LENVECTOR(len)-19+i]);
	}
  
	int* tmp = (int*)malloc(nb_integer*sizeof(int));
    for(int i=0;i<nb_integer;i++){
        tmp[i] = cpu_encrypted[i];
    }

    unsigned char *decryptedBuffer = convert_to_char(tmp,len);

    fp = fopen("decrypted_test.jpg", "wb");
    //fp = fopen("image_decrypted_test.jpg", "wb"); 
    fwrite(decryptedBuffer, sizeof(decryptedBuffer[0]), data.length/sizeof(decryptedBuffer[0]), fp);
    fclose(fp);
    
	
 
  return 0;
}
