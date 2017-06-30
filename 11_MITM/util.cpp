#include "../shared/Foper.cpp"
#include "../shared/des.h"
class util
{
public:
    std::vector<unsigned char> _2desEncrypted;
    des_class des;
    std::vector<unsigned char> &doPrepare()
    {
        Foper _inputfile;
        _inputfile.open(string("input.txt"));
        return _inputfile.GetData();
    }
    void _des(unsigned char key[8], std::vector<unsigned char> _data, std::vector<unsigned char> &_result)
    {
        des.set_key(key);
        des.vector_ecb_encode(_data, _result);
    }
    void _undes(unsigned char key[8], std::vector<unsigned char> _data, std::vector<unsigned char> &_result)
    {
        des.set_key(key);
        des.vector_ecb_decode(_data, _result);
    }
    std::vector<unsigned char> _des2(unsigned char key[8], unsigned char key2[8], std::vector<unsigned char> &_data, std::vector<unsigned char> _result)
    {
        _des(key,_data,_result);
        cout << _data.data() << "!!!!!!!!!!!!\n";
        //for (int i = 0; i<14; i++)cout << key2[i];
        //for (int i = 0; i<14;i++)cout<<_result.data()<<'_'<<_data.data()<< std::endl;
        _des(key2,_result,_2desEncrypted);
        
        //for (int i = 0; i < _inputfile.GetData().size(); i++)cout << _inputfile.GetData().at(i);
        /*cout << " ---------------------- ";
        */
        for (int i = 0; i < _result.size(); i++)cout << _result.at(i) << "!!!!!!!!!!!!\n";;
        return _result;
        //des.set_key(key2);
        //des.vector_ecb_decode(data, result);
        //des.set_key(key);
        //des.vector_ecb_decode(result, data);
        //Foper out;
        //out.GetData() = data;
        //out.write(string("res.txt"));
    }
};