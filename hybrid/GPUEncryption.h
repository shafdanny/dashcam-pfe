#include <thread>
#include "BlockingQueue.h"
#include "utils.h"
#include <iostream>

using namespace std;

class GPUEncryption
{
    
    public:
        GPUEncryption(BlockingQueue<Data>* _queue);
        virtual void start();
        virtual void run();
        
    protected:
        BlockingQueue<Data>* queue;
        thread t;
};

