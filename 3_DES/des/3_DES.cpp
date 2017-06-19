#include "des.cpp"
int main()
{
    des_class des;
    //des.gen_key3();
    //des.cbc_encode("input.docx","out");
    //des.cbc_decode("out", "result.docx");
    /*des.gen_key();
    des.EncryptCBC("input.txt", "result.txt", "helloew");
    des.DecryptCBC("result.txt", "output.docx", "helloew");*/
    /*unsigned char* val = (unsigned char*)"helloworld";
    std::vector<unsigned char> Data;
    for (int i = 0; i < des._EncData.size(); i++)Data.push_back(val[i]);
    des.EncryptCBC(Data, "helloke");
    for (int i = 0; i < Data.size(); i++)cout << Data.at(i);
    cout << std::endl;
    for (int i = 0; i < des._EncData.size(); i++)cout << des._EncData.at(i);*/
    system("cls");
    bool running = true;
    while (running) {
        cout << "Input command (gen_key/set_key/create_key/ecb_enc/ecb_dec): " << std::endl;
        string mode = "";
        cin >> mode;
        cout << "Mode: " << mode << std::endl;
        if (mode == "set_key")
        {
            char key_arr[32];
            cin >> key_arr;
            if (!des.set_key((unsigned char*)key_arr))return -2;
            cout << "Key set.\nInput mode: ";
            cin >> mode;
            cout << "Mode: " << mode << std::endl;
        }
        else
            if (mode == "gen_key" || mode == "create_key")
            {
                des.gen_key();
                if (mode == "create_key")des.create_key();
                cout << "Key set.\nInput mode: ";
                cin >> mode;
                cout << "Mode: " << mode << std::endl;
            }
        string source = "";
        cout << "Source: ";
        cin >> source;
        cout << std::endl;
        string dest = "";
        cout << "Destination: ";
        cin >> dest;
        cout << std::endl;
        if (mode == "ecb_enc") {
            des.ecb_encode(source, dest);
        }
        else
            if (mode == "ecb_dec") {
                des.ecb_decode(source, dest);
            }
            else
                if (mode == "cbc_enc") {
                    //des.cbc_encode(source, dest);
                }
                else
                    if (mode == "ecb_dec") {
                        //des.ecb_decode(source, dest);
                    }
                    else running = false;
    }
    system("pause");
    return 0;
}