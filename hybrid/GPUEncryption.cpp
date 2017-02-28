#include "GPUEncryption.h"
#include "utils.h"
#include "QPULib.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

void xorFunction2(Ptr<Int> p, Ptr<Int> q, Ptr<Int> r, Int n);

void GPUEncryption::run(){
    Data data;
    auto k = compile(xorFunction2);
    // Set the number of QPUs to use
    k.setNumQPUs(8);
    while(true){
        data = queue->pop();
        //cout << data.msg << "key " << data.key << "cmp " << data.len << endl;
        int size = LEN(data.len);
        // Allocate and initialise arrays shared between ARM and GPU
        SharedArray<int> message(size), key(size), encrypted(size);
        //srand(0);

        int* integer_msg = convert_to_integer(data.msg,data.len);
        int* integer_key = convert_to_integer(data.key,data.len);

        for(int i=0;i<size;i++) {
            message[i] = integer_msg[i];
            key[i] = integer_key[i];
        }
        
        // Invoke the kernel and display the result
        k(&message, &key, &encrypted, size);
        unsigned int* tmp = (unsigned int*)malloc(size*sizeof(int));
        for(int i=0;i<size;i++){
            tmp[i]=encrypted[i];
        }
        unsigned char *decryptedBuffer = convert_to_char(tmp,data.len);
        fwrite(decryptedBuffer, sizeof(decryptedBuffer[0]), data.len, stdout);
        fflush(stdout);
		
		free(data.msg);
		free(data.key);
		//cout << data.msg << endl;
    }
}

GPUEncryption::GPUEncryption(BlockingQueue<Data>* _queue)
{
    queue = _queue;
}


void GPUEncryption::start(){
    t = std::thread(&GPUEncryption::run, this);
}

void xorFunction2(Ptr<Int> p, Ptr<Int> q, Ptr<Int> r, Int n)
{
  Int inc = numQPUs() << 4;

  Ptr<Int> a = p + index() + (me() << 4);
  Ptr<Int> b = q + index() + (me() << 4);
  Ptr<Int> c = r + index() + (me() << 4);
  gather(a);gather(b);
  
  Int pOld, qOld, rOld;

  For(Int i = 0, i<n-1, i=i+inc)
    gather(a+inc); gather(b+inc);
    receive(pOld); receive(qOld);
    
    store(pOld^qOld, c);
    
    a = a+inc; b=b+inc; c=c+inc;
  End
  receive(pOld); receive(qOld);
  store(pOld^qOld, c);
}