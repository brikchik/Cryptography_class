#include "../SHARED/des.h"
int main()
{
    des_class des;
    system("cls");
    bool running = true;
    while (running) {
        cout << "Input command (gen_key/set_key/create_key/ecb_enc/ecb_dec/des3_ecb_enc/des3_ecb_dec): " << std::endl;
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
            else
            {
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
                    else if (mode == "des3_ecb_enc") {
                        des.des3_ecb_encode(source, dest);
                    }
                    else
                        if (mode == "des3_ecb_dec3") {
                            des.des3_ecb_decode(source, dest);
                        };
            }
        running = false;
    }
    system("pause");
    return 0;
};