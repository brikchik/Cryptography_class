#include <openssl/aes.h> 
#include <Windows.h>
#include <iostream>
static const unsigned char key[] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
         0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
           };
int main()
{
    unsigned char text[16] = "virident";
    unsigned char out[16];
    unsigned char decout[16];
    AES_KEY wctx;
    AES_set_encrypt_key(key, 128, &wctx);
    AES_encrypt(text, out, &wctx);
    std::cout << text << std::endl;
    std::cout<<"Encrypted data: "<< out+'\0'<<"\n";
    AES_decrypt(out, decout, &wctx);
    std::cout<<"Decrypted o/p: "<< decout+'\0'<<"\n";
    system("pause");
    return 0;
}
