#include "CPUEncryption.h"
#include <stdlib.h>
#include <stdio.h>


void CPUEncryption::start(){
    t = std::thread(&CPUEncryption::run, this);
}


void CPUEncryption::run(){
    Data data;
    while(true){
        data = queue->pop();
        unsigned char decrypted[data.len];
        
        for(int i=0;i<data.len;i++){
            decrypted[i] = data.msg[i] ^ data.key[i];
        }
        
        fwrite(decrypted, sizeof(decrypted[0]), data.len, stdout);
        fflush(stdout);
		
		free(data.msg);
		free(data.key);
        //free(decrypted);
		//cout << data.msg << endl;
    }
}