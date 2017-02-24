#include "QPULib.h"

// Define function that runs on the GPU.

void hello(Ptr<Int> p,Ptr<Int> c)
{
    store(*p,c);
  //*p = 1;
}

int main()
{
  // Construct kernel
  auto k = compile(hello);

  // Allocate and initialise array shared between ARM and GPU
  SharedArray<int> array(16),result(16);
  for (int i = 0; i < 16; i++)
    array[i] = 100;
    printf("initial value %x\n",array[0]);
  // Invoke the kernel and display the result
  k(&array,&result);
  for (int i = 0; i < 16; i++) {
    printf("%i: %i\n", i, array[i]);
  }
  printf("after value %x\n",result[0]);
  
  return 0;
}
