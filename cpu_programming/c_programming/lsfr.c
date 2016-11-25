#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

typedef struct{
    char* buf;
    int length;
} Data;

//generate the polynom from an hexa integer
int* generate_lsfr(unsigned int key)
{
	int* polynom = (int*)malloc(32*sizeof(int));
	int bit;
	int cmp=0;
	int i= 0;
	printf("\nkey %08X\n",key);
	//get recursively the first bit of our integer if it equals one we store its place in the original integer
	while(key != 0){
		//printf("key: %08X cmp %d\n",key,cmp);
		bit = key & 1;	
		//printf("value %d\n",bit);
		if(bit == 1){
			polynom[i] = cmp;
			i++;
		}
		cmp++;
		//add a 0 to the right
		key = key >> 1;
	}
	printf("end\n");
	if(i<31){
		polynom[i] = -1;
	}
	printf("end");
	return polynom;
}

//apply lsfr and output nb random byte, change 8 to 32 to have full integer
void lsfr(uint32_t seed,int tab[],int length,int nb){
	int i;
	int k;
	int j;
	int bit;
	uint8_t byte;
	uint32_t regist = seed;
	printf("\nregister state %08X\n",regist);
	for(i=0;i<nb;i++){
		byte = 0;
		//product a byte
		for(j=0;j<8;j++){
			//we place shift tab[0] times to the right, to get the deisred bit in first place
			bit = regist >> tab[0];
			//product a bit
			for(k=1;k<length;k++){
				bit = bit ^ (regist >> tab[k]);
			}
			bit  = bit & 1;
			//printf("producted %08X\n",bit);
			byte |= (bit << 7-j);
			regist = (regist >> 1) | (bit << 31);
			
		}
		printf("\nobtained byte %08X\n",byte);
		//printf("\nregister state %08X\n",regist);
	}
}

int get_random_integer(uint32_t* choice,uint32_t* state,int tab[],int length,int nb)
{
	int result = 0;
	int byte;
	int i,k;
	int rand = *choice >> tab[0];
	int bit = *state >> tab[0];
	//product a bit
	i = 0;
	while(i<32){
		for(k=1;k<length;k++){
			rand = rand ^ (*choice >> tab[k]);
			bit = bit ^ (*state >> tab[k]);
		}
		rand  = rand & 1;
		bit = bit & 1;
		//shift the 2 register
		*choice = (*choice >> 1) | (rand << 31);
		*state = (*state >> 1) | (bit << 31);
		if(rand == 1){
			result |= (bit << 31-i);
			i++;
		}
	}
	return result;
}
void test_lfsr()
{
	int i = 0;
	int length;
	uint16_t seed = 0xACE1u;
	int key = 0x800001A6;
	int* polynom = generate_lsfr(key);
	printf("bits used: ");
	while(polynom[i] != -1){
		printf("%d",polynom[i] && i<32);
		i++;
	}
	length = i;
	printf("\n");
}

int main()
{
	int length;
	int i = 0;
	uint32_t seed = 0x69ED9475u;
	//20D
	unsigned int key = 0x800001A6;
	printf("----------------test polynom extraction----------------");
	test_lfsr();
	printf("--------------test generation random byte--------------");
	int* polynom = generate_lsfr(key);
	while(polynom[i] != -1 && i<32){
		i++;
	}
	length = i;
	lsfr(seed,polynom,length,30);
	return 0;
}