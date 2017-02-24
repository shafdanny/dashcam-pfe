#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


class Trivium{
    
    public:
        static int* m_key;
        static int* m_init;
        Trivium();
        unsigned char* cypher(unsigned char* buffer, int len);
        void initiation(int* key = m_key, int* init = m_init);
    
    
    private:
        unsigned int get_integer();
        int* r1;
        int* r2;
        int* r3;
        int offset_integer;
    
};

