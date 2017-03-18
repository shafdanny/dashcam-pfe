# Secure Dashcam

## installation and dependencies

We use the QPULIB developped by broadcam to acces the gpu of the raspberry
see: https://github.com/mn416/QPULib
## Répartition des différentes versions

- gpu_programming: différentes versions du xor
Encrypt.cpp version 1 with bad convertion byte to integer
Encrypt_optimized version 2 with better convertion and use of the FIFO queues of the QPU

usage: go in Tests folder, with a command line type make QPU=1 Encrypt or make QPU=1 Encrypt2

- gpu_encryption

Trivium_encryption.cpp Trivium on gpu you can specify the file and the number of cure you want to use
Trivum_pad_encryption Generate a key on CPU, XOR on GPU
Triviumv2 trying to reduce the number of operations on the GPU by storing the registers on one vector instead of 10, not working

usage: go in Tests folder, with a command line type make QPU=1 Trivium or make QPU=1 Trivium_pad

- hybrid

TriviumKeygen.cpp file where we generate the key and the data we pass to the gpu using a blocking queue, Producer/consumer architecture
Trivium.cpp definition of Trivum class
GPUEncryption.cpp read from Queue and XOR the data put it on stdout
CPUEncryption.cpp Trivium on CPU

usage: 
GPU with command line type make QPU=1 TriviumKeygen then cat test.jpg | TriviumKeygen > image_crypted.jpg
CPU with command line type compile with g++ -o TriviumKeygenCPU TriviumKeygen.cpp BlockingQueue.h trivium.cpp GPUEncryption.cpp -std=c++11 -pthread
then cat test.jpg | TriviumKeygenCPU > image_crypted.jpg

- camera

Implementation of our streaming solution.
Take the stream from the camera and send them with a basic TCP socket.

usage run the client with qt speciify the raspberry ip then python camerasocket.py | ./encryption | python mysocket.py


