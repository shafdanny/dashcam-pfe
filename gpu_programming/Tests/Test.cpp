#include <stdlib.h>
#include "QPULib.h"
#include <time.h>

void do_things(Ptr<Int>p)
{
	*p = 1;
}
void xorFunction(Ptr<Int> p,Ptr<Int>q, Ptr<Int> test)
{
	int i;
	Int a = *p;
	Int b = *q;
	*p = rotate(*p,1);
	do_things(p);
	do_things(p);
	// *test = *test <<  3;
	// for(i=0;i<12;i++){
		// if(i==6){
			// *p = a + i;
		// }
	// }
}

int main()
{
	auto k = compile(xorFunction);
	SharedArray<int> a(16),b(16),test(17);
	test[0] = 1;
	test[1] = 2;
	test[16] = 3;
	srand(0);
	for (int i = 0; i < 4; i++) {
		a[i] = i;
	}
	for (int i = 0; i < 16; i++) {
		b[i] = 1;
	}
	printf("before\n");
	for (int i = 0; i < 16; i++) {
		printf("val %d\n",a[i]);
	}
	k.setNumQPUs(1);
	k(&a,&b,&test);
	printf("after\n");
	for (int i = 0; i < 16; i++) {
		printf("val %d\n",a[i]);
	}
	printf("test value%d %d %d\n",test[0],test[1],test[16]);
}
