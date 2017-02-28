#include "GPUEncryption.h"
#include <unistd.h>


//g++ -o test test.cpp GPUEncryption.cpp BlockingQueue.h -std=c++11

int main(){
    
    BlockingQueue<Data> queue;
    GPUEncryption gpu = GPUEncryption(&queue);
    cout << "launching thread" << endl;
    gpu.start();
    cout << "after launching" << endl; 
    int cmp;
    while(true){
        cmp ++;
        Data data;
        data.msg = "salut connard ";
        data.key = "pd";
        data.len = cmp;
        queue.push(data);
        sleep(1);
    }
    
    
}