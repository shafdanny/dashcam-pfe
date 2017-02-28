#include<thread>
#include "BlockingQueue.h"
#include <iostream>

using namespace std;

class GPUEncryption
{
    
    public:
        GPUEncryption(BlockingQueue<Data>* _queue);
        void start();
        void run();
        
    private:
        BlockingQueue<Data>* queue;
        thread t;
};

