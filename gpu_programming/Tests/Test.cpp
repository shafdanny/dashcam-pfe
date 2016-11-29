#include <stdlib.h>
#include "QPULib.h"
#include <time.h>


void xorFunction(Ptr<Int> p,Ptr<Int>q)
{
	int i;
	Int a = *p;
	Int b = *q;
	for(i=0;i<12;i++){
		if(i==6){
			*p = a + i;
		}
	}
}

int main()
{
	auto k = compile(xorFunction);
	SharedArray<int> a(16),b(16);
	srand(0);
	for (int i = 0; i < 16; i++) {
		a[i] = 6;
	}
	for (int i = 0; i < 16; i++) {
		b[i] = 1;
	}
	k.setNumQPUs(1);
	k(&a,&b);
	for (int i = 0; i < 16; i++) {
		printf("val %d\n",a[i]);
	}
}
