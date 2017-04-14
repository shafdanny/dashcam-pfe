#include <thread>
#include "GPUEncryption.h"

using namespace std;

class CPUEncryption : public GPUEncryption
{
    
    public:
        CPUEncryption(BlockingQueue<Data>* _queue):GPUEncryption(_queue){}
        virtual void start() override;
        virtual void run() override;
        
        
};

