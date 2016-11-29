#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iomanip> 

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
    std::cout << "char out : " << std::hex << (int)out << std::endl;
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
            std::cout << "data : " << std::hex << (int)data << std::endl;
	return i/2;
}

int main(int argc, char **argv)
{
	std::string key = "22";
    std::string fileNameIn = "carre_rouge.png";
    std::string fileNameOut = "carre_rouge_crypted.png";
	bool hex = false;
	int len=0;
	unsigned char keydata[256];
    
	len=gethexkey(keydata, key);

	State bytestream (keydata, len);
    
    std::cout << "keyData : " << std::hex << keydata << std::endl;

	std::fstream infile;
	infile.open(fileNameIn.c_str(), std::ios::in | std::ios::binary);
	if(!infile.is_open())
	{
		std::cout << fileNameIn << " does not exist" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::fstream outfile;
	outfile.open(fileNameOut.c_str(), std::ios::in);
	if(outfile.is_open()) //file we are going to write to exists!
	{
		std::cout << fileNameOut << " already exists, aborting to preserve it" << std::endl;
		exit(EXIT_FAILURE);
	}

	outfile.close();
	outfile.open(fileNameOut.c_str(), std::ios::out | std::ios::binary);
	if(outfile.is_open()) //Test if we were able to open the file for writing
	{
		char inbyte;
		char outbyte;
		unsigned char streambyte;

		infile.get(inbyte);
         
		while(!infile.eof())
		{
			streambyte=bytestream.getbyte();
			outbyte=inbyte ^ streambyte;
            std::cout << std::hex << (int)streambyte << " ";
			outfile.put(outbyte);
			infile.get(inbyte);
		}
        std::cout << std::endl;
	}
	else
	{
		std::cout << "Could not open " << fileNameOut << " for writing\n" <<std::endl;
		exit(EXIT_FAILURE);
	}

	outfile.close();
	infile.close();

	std::cout << "Encryption finished, output to " << fileNameOut << std::endl;

	return 0;
}
