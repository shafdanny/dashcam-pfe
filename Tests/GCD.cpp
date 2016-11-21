#include <stdlib.h>
#include "QPULib.h"
#include <time.h>

void gcd(Ptr<Int> p, Ptr<Int> q, Ptr<Int> r)
{
  Int a = *p;
  Int b = *q;
  While (any(a != b))
    Where (a > b)
      a = a-b;
    End
    Where (a < b)
      b = b-a;
    End
  End
  *r = a;
}

int main()
{
  // Construct kernel
  auto k = compile(gcd);

  // Set the number of QPUs to use
  k.setNumQPUs(1);

  // Allocate and initialise arrays shared between ARM and GPU
  SharedArray<int> a(16), b(16), r(16);
  srand(0);
  for (int i = 0; i < 16; i++) {
    a[i] = 100 + (rand() % 100);
    b[i] = 100 + (rand() % 100);
  }

  clock_t t1=clock();
  // Invoke the kernel and display the result
  k(&a, &b, &r);
  clock_t t2=clock();
  double time = (double)(t2 - t1) / CLOCKS_PER_SEC;
  printf("The time taken is.. %f \n\n", time);

  for (int i = 0; i < 16; i++)
    printf("gcd(%i, %i) = %i\n", a[i], b[i], r[i]);
  
  return 0;
}
