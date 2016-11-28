#include <stdlib.h>
#include <stdio.h>
#include "RPiGPUEncrypt.h"

int main(void) {
    printf("Starting encryption test ... \n");

    encrypt_file("./images/bigger.jpg", "./images/encryptedGPU.jpg");
    encrypt_file("./images/encryptedGPU.jpg", "./images/decryptedGPU.jpg");

    return 0;
}