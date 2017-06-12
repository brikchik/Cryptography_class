#include "md5.h"
#include <iostream>
#include "../../Foper.cpp"
//#include <openssl/md5.h>
#define vector std::vector<unsigned char>
#define cout std::cout
#define cin std::cin
#define blockLength 120 //blockLength%10==0
//blockLength/10 bytes output for each block
vector funcIt(vector iv,vector block)//transforms blockLength bytes into blockLength/10 bytes 
{
    vector res;
    for (int i = 0; i < blockLength/10; i++)   
    {
        unsigned char symbol = 
            (block.at(i * 10)+block.at(i*10+2)+block.at(i * 10+4)+block.at(i * 10 + 6)+block.at(i * 10+8))
            ^
            (block.at(i * 10+1)+block.at(i * 10 + 3)+block.at(i * 10 + 5)+block.at(i * 10 + 7)+block.at(i * 10 + 9));
        symbol ^= iv.at(i);
        res.push_back(symbol);
    }
    return res;
}
vector Hash(vector file,bool useMD5) {
    vector result;
    vector iv = { 'a','m','c','d','e','f','a','b','l','d','e','f','a','b','c','d','k','f','a','n','c','d','e','f' };
    //24byte hash value. iv is defined in standard
    int size = file.size();
    int maxSize = size;
    while (maxSize % blockLength != 0)maxSize++;
    //240byte source block
    vector block;
    if (useMD5)
    {
        string x;
        for (int i = 0; i < maxSize / blockLength; i++)
        {
            for (int j = 0; j < blockLength; j++)x.append(1,file.at(j * blockLength + i));
            x = md5(x);
            for (int i = 0; i < blockLength/10; i++)result.push_back(x[i]);//taking first 24 bytes of md5 hash
        }
        return result;
        //MD5_CTX md5handler;
        //unsigned char x[blockLength];
        //unsigned char md5digest[MD5_DIGEST_LENGTH];        
        //for (int i = 0; i < maxSize / blockLength; i++)
        //{
        //    for (int j = 0; j < blockLength; j++)x[j]=file.at(j + blockLength * i);
        //    MD5_Init(&md5handler);
        //    MD5_Update(&md5handler, x, blockLength);
        //    MD5_Final(md5digest, &md5handler);
        //    for (int i = 0; i < blockLength/10; i++)result.push_back(md5digest[i]);//taking first 24 bytes of md5 hash
        //}
        //return result;
    }
    for (int j = 0; j < maxSize / blockLength; j++)
    {
        for (int i = 0; i < blockLength; i++)
        {
            if ((j * blockLength + i) < size)block.push_back(file.at(j * blockLength + i));
            else block.push_back(0);
        }
        iv = funcIt(iv, block);
        block.clear();
        for (int i = 0; (i < blockLength/10) && (i < size / blockLength); i++)
        {
            result.push_back(iv.at(i));
        }
    }
    return result;
}
int main()
{
    Foper file;
    Foper outfile;
    int choice=0;
    cout << "1-RHash, 2-default hash\n";
    cin >> choice;
    if (!file.open((string)"input.txt"))return 1;
    if (choice==1)
    {
        vector hash = Hash(file.GetData(),false);
        outfile.GetData() = hash;
        outfile.write((string)"hash.txt");
    }
    else
        if (choice == 2)
        {
            vector hash = Hash(file.GetData(), true);
            outfile.GetData() = hash;
            outfile.write((string)"hash.txt");
        };
    return 0;
}