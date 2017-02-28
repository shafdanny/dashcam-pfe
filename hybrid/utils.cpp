#include "utils.h"
#include <stdlib.h>
#include <time.h>

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
	if(remaining != 0){
        tab[quarter] = 0;
        for(i=0;i<remaining;i++){
            tab[quarter]|= (int)(buff[cmp+i] << (3-i)*8);
        }
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


