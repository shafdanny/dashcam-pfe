#include "QPULib.h"
#include <time.h>

void tri(Ptr<Int> p)
{
  p = p + (me() << 4);
  Int n = *p;
  Int sum = 0;
  While (any(n > 0))
    Where (n > 0)
      sum = sum+n;
      n = n-1;
    End
  End
  *p = sum;
}

int main()
{
  clock_t begin = clock();
  // Construct kernel
  auto k = compile(tri);

  // Use 4 QPUs
  k.setNumQPUs(4);

  // Allocate and initialise array shared between ARM and GPU
  SharedArray<int> array(64);
  for (int i = 0; i < 64; i++)
    array[i] = i;

  // Invoke the kernel and display the result
  k(&array);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("Time %f s \n\n", time_spent);
  
  for (int i = 0; i < 64; i++)
    printf("%i: %i\n", i, array[i]);
  
  return 0;
}
