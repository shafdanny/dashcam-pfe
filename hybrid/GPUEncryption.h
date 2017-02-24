#include<thread>
using namespace std;

class GPUEncryption
{
    
    public:
        GPUEncryption(Queue* _queue);
        
    private:
        Queue* queue;
        thread t;
}

