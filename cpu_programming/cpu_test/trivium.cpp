#include "trivium.h"
#include <iostream>

#define LEN(n) ((n%4 == 0)? n/4 : (n/4)+1)
#define LENVECTOR(n) ((n%16 == 0)? (n/16) : (n/16)+1)

using namespace std;

int* Trivium::m_key = (int*)calloc(3,sizeof(int));
int* Trivium::m_init = (int*)calloc(3,sizeof(int));

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
	}
	remaining = n%4;
	tab[quarter] = 0;
	for(i=0;i<remaining;i++){
		tab[quarter]|= (int)(buff[cmp+i] << (3-i)*8);
	}
	return tab;
}

//convert an int array to char* using each byte of the int array, CARE n IS THE SIZE OF THE DESIRED CHAR ARRAY not the INTEGER
unsigned char* convert_to_char(unsigned int* buff,int n)
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
	}
	for(i=0;i<remaining;i++){
		tab[cmp+i] = (unsigned char)(buff[quarter] >> (3-i)*8) & 0xFF;
	}
	return tab;
}

Trivium::Trivium()
{
    offset_integer = 0;
    // m_key = (int*)calloc(3,sizeof(int));
    // m_init = (int*)calloc(3,sizeof(int));
    r1 = (int*)calloc(3,sizeof(int));
    r2 = (int*)calloc(3,sizeof(int));
    r3 = (int*)calloc(4,sizeof(int));
  
    m_key[0] = 3861042005;//random_integer();
	m_key[1] = 1559349736;//random_integer();
	m_key[2] = 777729657;//random_integer()
	
	m_init[0] = 873716055;//random_integer();
	m_init[1] = 1559349736;//random_integer();
	m_init[2] = 3547487477;//random_integer();
}

/*
    Use one core of the cpu to cypher/decypher the data using the current state of our registers
    data is the buffer you want to cypher/decypher
    n size in char of your buffer
*/
unsigned char* Trivium::cypher(unsigned char* data,int n)
{
    int i,k;
    int nb_integer = LEN(n);
    int nb_vector = LENVECTOR(nb_integer);
    int* message = convert_to_integer(data, nb_vector*64);
    cout <<"len" << n << " " << nb_vector*16 << endl;
	unsigned int* res =(unsigned int*)calloc(nb_vector*16,sizeof(int));
    unsigned int* vector = (unsigned int*)malloc(16*sizeof(int));
    if(!res || !vector){
        cout << "mem faillure res" << endl;
        exit(0);
    }
	for(i=0;i<nb_vector;i++){
        for(k=0;k<16;k++){
            int key = get_integer();
            vector[15-k] = key;
        }
        for(k=0;k<16;k++){
            res[i*16+k] = message[i*16+k] ^ vector[k];
        }
        
	}
    offset_integer += nb_vector*16;
    unsigned char *decryptedBuffer = convert_to_char(res,n);
    free(message);
    free(res);
	return decryptedBuffer;
}
/*
    Iniatilise the register using the key and init vector
*/
void Trivium::initiation(int* key,int* init)
{
	int i;
	int len = 4*288;
	int t1,t2,t3;
	int bit1,bit2,bit3;
	(r1)[0] = key[0];
	(r1)[1] = key[1];
	(r2)[0] = init[0];
	(r2)[1] = init[1];
	
	cout << "regist  init: " << init[0] << " key: " << key[0] << endl;
	//on veut les 80 premiers bits de key et de init 64 + 16
	//les 16 premiers bits 
	(r1)[2] = key[2] >> 16;
	(r2)[2] = init[2] >> 16;
	//le reste a 0
	(r1)[2] = (r1)[2] << 16;
	(r2)[2] = (r2)[2] << 16;
	
	//les 3 derniers bits de r3 valent 1
	(r3)[3] = 8;	
	
	for(i=0;i<len;i++){
		// 66 ou 91 & 92 ou 93 ou 171
		t1 = ((r1)[2] >> 1) ^ ( ((r1)[2] >> 26) & ((r1)[2] >> 27) ) ^ ((r1)[2] >> 28) ^ ((r2)[2] >> 12);
		// 162 ou 175 et 176 ou 177 ou 264
		t2 = ((r2)[2] >> 3) ^ ( ((r2)[2] >> 16) & ((r2)[2] >> 17) ) ^ ((r2)[2] >> 18) ^ ((r3)[2] >> 21);
		// 243 ou 286 et 287 ou 288 ou 69
		t3 = ((r3)[2]) ^ ( ((r3)[3] >> 15) & ((r3)[3] >> 16) ) ^ ((r3)[3] >> 17) ^ ((r1)[2] >> 4);
		
		//on decale chacun des tableaux en recuperant le dernier bit du tableau précédant de chacun des tableaux [93,84,111]
		//tableau 1
		bit1 = ((r1)[0] >> 31) & 1;
		bit2 = ((r1)[1] >> 31) & 1;
		(r1)[0] = (t1 & 1) | ((r1)[0] << 1);
		(r1)[1] = (bit1 & 1) | ((r1)[1] << 1);
		(r1)[2] = (bit2 & 1) | ((r1)[2] << 1);
		
		//tableau 2
		bit1 = ((r2)[0] >> 31) & 1;
		bit2 = ((r2)[1] >> 31) & 1;
		(r2)[0] = (t2 & 1) | ((r2)[0] << 1);
		(r2)[1] = (bit1 & 1) | ((r2)[1] << 1);
		(r2)[2] = (bit2 & 1) | ((r2)[2] << 1);
		
		//tableau3
		bit1 = ((r3)[0] >> 31) & 1;
		bit2 = ((r3)[1] >> 31) & 1;
		bit3 = ((r3)[2] >> 31) & 1;
		(r3)[0] = (t3 & 1) | ((r3)[0] << 1);
		(r3)[1] = (bit1 & 1) | ((r3)[1] << 1);
		(r3)[2] = (bit2 & 1) | ((r3)[2] << 1);
		(r3)[3] = (bit3 & 1) | ((r3)[3] << 1);
	}
}

/*
    Return one integer using our register
*/
unsigned int Trivium::get_integer()
{
	int i;
	int t1,t2,t3;
	int bit1,bit2,bit3;
	unsigned int z = 0;
	
	for(i=0;i<32;i++){
		//66:2 ou 93:29
		t1 = (((r1)[2] >> 1) ^ ((r1)[2] >> 28)) & 1;
		//162:68:4 177:83
		t2 = (((r2)[2] >> 3) ^ ((r2)[2] >> 18)) & 1;
		//243:65:1 288:110:14
		t3 = (((r3)[2]) ^ ((r3)[3] >> 13)) & 1;
		
		//compute the output bit
		z = z << 1;
		z |= t1 ^ t2 ^ t3;
		
		//compute the 3 input bit
		//91 et 92 ou 171
		t1 ^= (((r1)[2] >> 26) & ((r1)[2] >> 27)) ^ ((r2)[2] >> 12);
		//175 et 176 ou 264
		t2 ^= (((r2)[2] >> 16) & ((r2)[2] >> 17)) ^ ((r3)[2] >> 21);
		//286 et 287 ou 69
		t3 ^= (((r3)[3] >> 11) & ((r3)[3] >> 12)) ^ ((r1)[2] >> 4);
		
		//rotate the registers
		//tableau1
		bit1 = ((r1)[0] >> 31) & 1;
		bit2 = ((r1)[1] >> 31) & 1;
		(r1)[0] = (t1 & 1) | ((r1)[0] << 1);
		(r1)[1] = (bit1 & 1) | ((r1)[1] << 1);
		(r1)[2] = (bit2 & 1) | ((r1)[2] << 1);
		
		//tableau 2
		bit1 = ((r2)[0] >> 31) & 1;
		bit2 = ((r2)[1] >> 31) & 1;
		(r2)[0] = (t2 & 1) | ((r2)[0] << 1);
		(r2)[1] = (bit1 & 1) | ((r2)[1] << 1);
		(r2)[2] = (bit2 & 1) | ((r2)[2] << 1);
		
		//tableau3
		bit1 = ((r3)[0] >> 31) & 1;
		bit2 = ((r3)[1] >> 31) & 1;
		bit3 = ((r3)[2] >> 31) & 1;
		(r3)[0] = (t3 & 1) | ((r3)[0] << 1);
		(r3)[1] = (bit1 & 1) | ((r3)[1] << 1);
		(r3)[2] = (bit2 & 1) | ((r3)[2] << 1);
		(r3)[3] = (bit3 & 1) | ((r3)[3] << 1);
		
	}
	return z;
}
