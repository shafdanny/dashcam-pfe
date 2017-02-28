#ifndef UTILS_H_
#define UTILS_H_


#define LEN(n) ((n%4 == 0)? n/4 : (n/4)+1)
#define LENVECTOR(n) ((n%16 == 0)? (n/16) : (n/16)+1)

int* convert_to_integer(unsigned char* buff,int n);
unsigned char* convert_to_char(unsigned int* buff,int n);

#endif