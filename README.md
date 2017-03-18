# Secure Dashcam

## installation and dependencies

We use the QPULIB developped by broadcam to acces the gpu of the raspberry
see: https://github.com/mn416/QPULib

## Répartition des différentes versions

- gpu_programming: différentes versions du xor
* __Encrypt.cpp__ version 1 with bad convertion byte to integer
* __Encrypt_optimized__ version 2 with better convertion and use of the FIFO queues of the QPU

* usage: go in Tests folder, with a command line type make QPU=1 Encrypt or make QPU=1 Encrypt2

- gpu_encryption

* __Trivium_encryption.cpp__ Trivium on gpu you can specify the file and the number of cure you want to use
* __Trivum_pad_encryption.cpp__ Generate a key on CPU, XOR on GPU
* __Triviumv2.cpp__ trying to reduce the number of operations on the GPU by storing the registers on one vector instead of 10, not working

usage: go in Tests folder, with a command line type make QPU=1 Trivium or make QPU=1 Trivium_pad

- hybrid

* __TriviumKeygen.cpp__ file where we generate the key and the data we pass to the gpu using a blocking queue, Producer/consumer architecture
* __Trivium.cpp__ definition of Trivum class
* __GPUEncryption.cpp__ read from Queue and XOR the data put it on stdout
* __CPUEncryption.cpp__ Trivium on CPU

* usage: 
  * GPU with command line type make QPU=1 TriviumKeygen then cat test.jpg | TriviumKeygen > image_crypted.jpg
  * CPU with command line type compile with g++ -o TriviumKeygenCPU TriviumKeygen.cpp BlockingQueue.h trivium.cpp GPUEncryption.cpp -std=c++11 -pthread then cat test.jpg | TriviumKeygenCPU > image_crypted.jpg

- camera

* Implementation of our streaming solution.
* Take the stream from the camera and send them with a basic TCP socket.

* __camerasocket.py__ capture the video stream
* __crypt.cpp__ cypher the flow
* __mysocket.py__ send it with TCP

* usage:
  * compile the C++ code with a command line g++ -o crypt crypt.cpp trivium_multi_core.cpp trivium.cpp 
  * run the client with qt specify the raspberry ip then python camerasocket.py | ./encryption | python mysocket.py


