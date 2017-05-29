#include <stdio.h> 
#include <iostream>
#include <openssl/aes.h>
#include <vector>
#include <fstream>
#include <string>
//#include <openssl/rand.h>
#include <Windows.h>
class Foper
{
private:
    std::vector<unsigned char> _data;
public:
    Foper()
    {
    }
    bool open(std::string &name)
    {
        std::ifstream _file(name, std::ios::in | std::ios::binary);
        if (!_file.is_open())
        {
            std::cout << "No such file: " << name << std::endl;
            return false;
        }

        _file.seekg(0, std::ios::end);
        int size = _file.tellg();
        _file.seekg(0, std::ios::beg);

        if (size != 0) {
            unsigned char byte;
            for (int i = 0; i < size; i++)
            {
                byte = (unsigned char)(_file.get());
                _data.push_back(byte);
            }
        }
        else {
            std::cout << "Empty file";
            _file.close();
        }
        return !_data.empty();
    }
    std::vector<unsigned char> &GetData() { return _data; }
    bool write(std::string &output)
    {
        std::ofstream outfile(output, std::fstream::out | std::fstream::trunc | std::fstream::binary);
        if (!outfile.is_open()) { std::cout << "Unable to open file for writing"; return false; };
        for (int i = 0; i < _data.size(); i++)
        {
            outfile.put((unsigned char)_data.at(i));
        }
        outfile.close();
        return true;
    }
};
class aes_class
{
public:
    aes_class() {}
    unsigned char key[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    bool set_key(unsigned char new_key[])
    {
        if (strlen((char*)new_key + '\0') < 16)return false;
        for (int i = 0; i < 16; i++)key[i] = new_key[i];
        return true;
    };
    bool ecb_encrypt(std::string path, std::string dest) {
        Foper inputfile;
        Foper destfile;
        if (!inputfile.open(path))return false;
        std::vector<unsigned char> text = inputfile.GetData();

        int paddingNumber = 0; //PKCS#7 padding
        while (((text.size() + paddingNumber) % 16) != 0)
        {
            paddingNumber++;
        }
        for (int i = 0; i < paddingNumber; i++)
        {
            text.push_back(paddingNumber);
        }
        unsigned char input[16];
        unsigned char enc_out[16];
        std::vector<unsigned char> result;
        AES_KEY enc_key;
        int length = text.size() - 1;
        int pos = 0;
        AES_set_encrypt_key(key, 128, &enc_key);
        for (int i = 0; i < length; i += 16) {

            for (int j = 0; j < 16; j++) {
                input[j] = text[i + j];
            }
            AES_encrypt(input, enc_out, &enc_key);
            for (int j = 0; j < 16; j++) {
                result.push_back(enc_out[j]);
                pos++;
            }
        }
        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        return true;
    }
    bool ecb_decrypt(std::string path, std::string dest) {
        Foper inputfile;
        Foper destfile;
        if (!inputfile.open(path))return false;
        std::vector<unsigned char> text = inputfile.GetData();
        unsigned char input[16];
        unsigned char enc_out[16];
        std::vector<unsigned char> result;
        AES_KEY dec_key;
        int length = text.size() - 1;
        int pos = 0;
        AES_set_decrypt_key(key, 128, &dec_key);
        for (int i = 0; i < length; i += 16) {
            for (int j = 0; j < 16; j++) {
                input[j] = text[i + j];
            }
            AES_decrypt(input, enc_out, &dec_key);
            for (int j = 0; j < 16; j++) {
                result.push_back(enc_out[j]);
                pos++;
            }
        }
        //PKCS#7 padding
        int paddingNumber = result.back();
        if (result.at(result.size() - paddingNumber) == paddingNumber)
            result._Pop_back_n(paddingNumber);

        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        return true;
    }
};
int main()
{
    aes_class aes;
    std::cout << "Input command (set_key/ecb_ecn/ecb_dec/etc.): " << std::endl;
    std::string mode = "";
    std::cin >> mode;
    std::cout << "Mode: " << mode << std::endl;
    if (mode == "set_key")
    {
        char key_arr[32];
        std::cin >> key_arr;
        if (!aes.set_key((unsigned char*)key_arr))return -2;
        std::cout << "Key set.\nInput mode: ";
        std::cin >> mode;
        std::cout << "Mode: " << mode << std::endl;
    }
    std::string source = "";
    std::cin >> source;
    std::cout << "Source: " << source << std::endl;
    std::string dest = "";
    std::cin >> dest;
    std::cout << "Destination: " << dest << std::endl;
    if (mode == "ecb_enc") {
        aes.ecb_encrypt(source, dest);
    }
    if (mode == "ecb_dec") {
        aes.ecb_decrypt(source, dest);
    }
    system("pause");
    return 0;
}
//
//#include <openssl/conf.h>
//#include <openssl/evp.h>
//#include <openssl/err.h>
//void handleErrors(void)
//{
//    ERR_print_errors_fp(stderr);
//    abort();
//}
//int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
//    unsigned char *iv, unsigned char *ciphertext)
//{
//    EVP_CIPHER_CTX *ctx;
//
//    int len;
//
//    int ciphertext_len;
//
//    /* Create and initialise the context */
//    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
//
//    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
//    * and IV size appropriate for your cipher
//    * In this example we are using 256 bit AES (i.e. a 256 bit key). The
//    * IV size for *most* modes is the same as the block size. For AES this
//    * is 128 bits */
//    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
//        handleErrors();
//
//    /* Provide the message to be encrypted, and obtain the encrypted output.
//    * EVP_EncryptUpdate can be called multiple times if necessary
//    */
//    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
//        handleErrors();
//    ciphertext_len = len;
//
//    /* Finalise the encryption. Further ciphertext bytes may be written at
//    * this stage.
//    */
//    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
//    ciphertext_len += len;
//
//    /* Clean up */
//    EVP_CIPHER_CTX_free(ctx);
//
//    return ciphertext_len;
//}
//int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
//    unsigned char *iv, unsigned char *plaintext)
//{
//    EVP_CIPHER_CTX *ctx;
//
//    int len;
//
//    int plaintext_len;
//
//    /* Create and initialise the context */
//    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
//
//    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
//    * and IV size appropriate for your cipher
//    * In this example we are using 256 bit AES (i.e. a 256 bit key). The
//    * IV size for *most* modes is the same as the block size. For AES this
//    * is 128 bits */
//    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
//        handleErrors();
//
//    /* Provide the message to be decrypted, and obtain the plaintext output.
//    * EVP_DecryptUpdate can be called multiple times if necessary
//    */
//    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
//        handleErrors();
//    plaintext_len = len;
//
//    /* Finalise the decryption. Further plaintext bytes may be written at
//    * this stage.
//    */
//    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
//    plaintext_len += len;
//
//    /* Clean up */
//    EVP_CIPHER_CTX_free(ctx);
//
//    return plaintext_len;
//}
//int main(void)
//{
//    /* Set up the key and iv. Do I need to say to not hard code these in a
//    * real application? :-)
//    */
//
//    /* A 256 bit key */
//    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
//
//    /* A 128 bit IV */
//    unsigned char *iv = (unsigned char *)"0123456789012345";
//
//    /* Message to be encrypted */
//    unsigned char *plaintext =
//        (unsigned char *)"The quick brown fox jumps over the lazy dog";
//
//    /* Buffer for ciphertext. Ensure the buffer is long enough for the
//    * ciphertext which may be longer than the plaintext, dependant on the
//    * algorithm and mode
//    */
//    unsigned char ciphertext[128];
//
//    /* Buffer for the decrypted text */
//    unsigned char decryptedtext[128];
//
//    int decryptedtext_len, ciphertext_len;
//
//    /* Initialise the library */
//    ERR_load_crypto_strings();
//    OpenSSL_add_all_algorithms();
//    OPENSSL_config(NULL);
//
//    /* Encrypt the plaintext */
//    ciphertext_len = encrypt(plaintext, strlen((char *)plaintext), key, iv,
//        ciphertext);
//
//    /* Do something useful with the ciphertext here */
//    printf("Ciphertext is:\n");
//    BIO_dump_fp(stdout, (const char *)ciphertext, ciphertext_len);
//
//    /* Decrypt the ciphertext */
//    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
//        decryptedtext);
//    /* Add a NULL terminator. We are expecting printable text */
//    decryptedtext[decryptedtext_len] = '\0';
//    /* Show the decrypted text */
//    printf("Decrypted text is:\n");
//    printf("%s\n", decryptedtext);
//    /* Clean up */
//    EVP_cleanup();
//    ERR_free_strings();
//
//    return 0;
//    system("pause");
//}
//
