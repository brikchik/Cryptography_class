#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <openssl/des.h>
#include <openssl/rand.h>

#define BUFSIZE 64 
class des {
private:
    unsigned char in[BUFSIZE], out[BUFSIZE], back[BUFSIZE];
    unsigned char *e = out;
    DES_cblock key;
    DES_cblock seed = { 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 };
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
        RAND_seed(seed, sizeof(DES_cblock));
        DES_random_key(&key);
        DES_set_key((DES_cblock *)key, &keysched);
    }
    bool encode(char* data) {
        if (data == NULL)return false;
        if (cipher_Type == "ecb") {
            strcpy((char*)in, data);
            printf("Plaintext: [%s]\n", in);

            for (int i = 0; i < strlen(data)/8; i++) {
                DES_ecb_encrypt((DES_cblock *)in, (DES_cblock *)out, &keysched, DES_ENCRYPT);
            }
            

            printf("Ciphertext:");
            while (*e) printf(" [%02x]", *e++);
            printf("\n");
            return true;
        }
        return false;
    }
    bool decode(int size) {
        for (int i = 0; i < size / 8; i++) {
            DES_ecb_encrypt((DES_cblock *)out, (DES_cblock *)back, &keysched, DES_DECRYPT);
        }
        printf("Decrypted Text: [%s]\n", back);
        return true;
    }
};
int main(int argc, char** args)
{
    des cipher("ecb");
    cipher.gen_key();
    if(!cipher.encode("Ôèçè÷åñêèéqwerty"))return -1;
    cipher.decode(strlen("Ôèçè÷åñêèéqwerty"));
    system("pause");
    return(0);
}