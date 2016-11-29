#include <iostream>
#include <fstream>

int main()
{
    std::ifstream keyFile("../gpu_programming/Tests/test_image.key", std::ios::in | std::ios::binary);
    std::ifstream image("../gpu_programming/Tests/test_image.jpg", std::ios::in | std::ios::binary);
    std::ofstream imageCrypted("../gpu_programming/Tests/test_image_crypted.jpg", std::ios::out | std::ios::binary);
    if (!imageCrypted)
    {
        std::cout << "Impossible de creer le fichier de sortie" << std::endl;
    }
    unsigned char bImage, bKey, bCrypted;
    while (image)
    {
        image.read(reinterpret_cast<char*>(&bImage), 1);
        keyFile.read(reinterpret_cast<char*>(&bKey), 1);
        bCrypted = bImage ^ bKey;
        unsigned char* bChar = &bCrypted;
        std::cout << (unsigned int)(*bChar) << " ";
        imageCrypted.write(reinterpret_cast<char*>(bChar), 1);
    }
    std::cout << std::endl;
    imageCrypted.close();
    return 0;
}