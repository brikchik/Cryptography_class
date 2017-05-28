#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <iostream>
#define BUFSIZE 64 
class des {
private:
    unsigned char in[BUFSIZE], out[BUFSIZE], back[BUFSIZE];
    unsigned char *e = out;
    DES_cblock key;
    DES_key_schedule keysched;
    char* cipher_Type="nothing";
public:
    des(char* type) {
        cipher_Type = type;
        memset(in, 0, sizeof(in));
        memset(out, 0, sizeof(out));
        memset(back, 0, sizeof(back));
    }
    void gen_key() {
        DES_random_key(&key);
        std::cout << "KEY: " << key << std::endl;
        DES_set_key((DES_cblock *)key, &keysched);
    }
    bool encode(char* data) {
        if (data == NULL)return false;
        if (cipher_Type == "ecb") {
            strcpy((char*)in, data);
            printf("Plaintext: [%s]\n", in);

      
                DES_ecb_encrypt((DES_cblock *)in, (DES_cblock *)out, &keysched, DES_ENCRYPT);
            
            

            printf("Ciphertext:");
            while (*e) printf(" [%02x]", *e++);
            printf("\n");
            return true;
        }
        return false;
    }
    bool decode(int size) {

            DES_ecb_encrypt((DES_cblock *)out, (DES_cblock *)back, &keysched, DES_DECRYPT);

        printf("Decrypted Text: [%s]\n", back);
        return true;
    }
};
int main(int argc, char** args)
{
    des cipher("ecb");
    cipher.gen_key();
    if(!cipher.encode("textb"))return -1;
    cipher.decode(strlen("textb"));
    system("pause");
    return(0);
}