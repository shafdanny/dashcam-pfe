#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned int random_integer()
{
	int i;
	unsigned int r = 0;
	unsigned char bits[4];
	for(i=0;i<4;i++){
		bits[i] = rand()%256;
		r |= bits[i] << (3-i)*8;
	}
	return r;
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

/**
* Generate a pseudo random int[] using trivium
* key and init must have a minimum of 80 byte length, 3 integer
*/
int* generate_random_key(int len,int* key,int* init)
{
	int i;
	int* res = (int*)malloc(len*sizeof(int));
	int* regist1 = calloc(3,sizeof(int));
	int* regist2 = calloc(3,sizeof(int));
	int* regist3 = calloc(4,sizeof(int));
	printf("\ninit: %d key: %d\n",init[0],key[0]);
	initiation(&regist1,&regist2,&regist3,key,init);
	for(i=0;i<len;i++){
		res[i] = get_integer(&regist1,&regist2,&regist3);
	}
	return res;
}
int main()
{
	int i;
	unsigned int* key = calloc(3,sizeof(int));
	unsigned int* init = calloc(3,sizeof(int));
	
	key[0] = 3861042005;//random_integer();
	key[1] = 1559349736;//random_integer();
	key[2] = 777729657;//random_integer()
	
	init[0] = 873716055;//random_integer();
	init[1] = 1559349736;//random_integer();
	init[2] = 3547487477;//random_integer();
	
	// int* regist1 = calloc(3,sizeof(int));
	// int* regist2 = calloc(3,sizeof(int));
	// int* regist3 = calloc(4,sizeof(int));
	// printf("\ninit: %d key: %d\n",init[0],key[0]);
	// initiation(&regist1,&regist2,&regist3,key,init);
	// for(i=0;i<16;i++){
		// printf("random int  %u\n",(unsigned int)get_integer(&regist1,&regist2,&regist3));
	// }
	int* res = generate_random_key(50,key,init);
	for(i=0;i<50;i++){
		printf("random int %u\n",res[i]);
	}
	int test = 8;
	printf("test %x\n",test >> 3);
	return 0;
}