#include "imagedecrypter.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <qDebug>

class State
{
    unsigned char s[256];
    int i, j;

    void swap(int a, int b);

    public:
    unsigned char getbyte(void);
    State(unsigned char key[], int length );
};

State::State(unsigned char key[], int length)
{
    for(int k=0; k<256; k++)
    {
        s[k]=k;
    }

    j=0;

    for(i=0; i<256 ; i++)
    {
        j=(j + s[i] + key[i % length]) % 256;
        swap(i, j);
    }

    i=j=0;
}

void State::swap(int a, int b)
{
    unsigned char temp= s[i];
    s[i]=s[j];
    s[j]=temp;
}

unsigned char State::getbyte(void)
{
    i=(i+1)%256;
    j=(j+s[i])%256;
    swap(i, j);
    int index=(s[i]+s[j])%256;
    return s[index];
}

void gethexdigit(char in, unsigned char & out)
{
    std::cout << "char in : " << std::hex << (int)in;
    if(in>='0' && in<='9')
    {
        out += in-'0';
    }
    else if(in>='a' && in<='f')
    {
        out += in -'a' + 10;
    }
    else
    {
        std::cout << "Hex key contains letter outside range 0-9 or a-z: " << in << std::endl;
        exit(EXIT_FAILURE);
    }
}

int gethexkey(unsigned char data[], std::string key)
{
    if(key.length() % 2) //key must be of even length if it's hex
    {
        std::cout << "Hex key must have an even number of characters" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(key.length() > 512)
    {
        std::cout << "Hex key cannot be longer than 512 characters long" << std::endl;
        exit(EXIT_FAILURE);
    }

    unsigned char byte;
    size_t i;

    for(i=0; i < key.length(); i++)
    {
        gethexdigit(key[i], byte);
        byte <<= 4;
        i++;
        gethexdigit(key[i], byte);
        data[(i-1)/2]=byte;
    }
    return i/2;
}


ImageDecrypter::ImageDecrypter(QByteArray cryptedData, QObject *parent) : QObject(parent)
{
        std::string key = "22";
        int len=0;
        unsigned char keydata[256];

        len=gethexkey(keydata, key);

        State bytestream (keydata, len);
        std::cout << "keyData : " <<keydata << std::endl;
        unsigned char streamByte;
        char outbyte;
        for (int i = 0; i < cryptedData.size(); i++)
        {
            streamByte=bytestream.getbyte();
            std::cout << std::hex <<(int)streamByte << " ";
            outbyte = cryptedData.at(i) ^ streamByte;
            decryptedData.append(outbyte);
        }
        std::cout << std::endl;

    //Génération de la clé à partir de la seed
    //On décrypte avec un XOR sur la clé et les données
    emit decrypted();

}

QByteArray ImageDecrypter::getDecryptedData()
{
    return decryptedData;
}
