#include "../../Foper.cpp"
#include "../../3_DES/des/des.cpp"
class util
{
public:
    Foper _inputfile;
    std::vector<byte> _2desEncrypted;
    std::vector<byte> _data;
    std::vector<byte> _result;
    des_class des;
    void restoreDefaults()
    {
        _data = std::vector<byte>(_inputfile.GetData());
        _result = std::vector<byte>(_2desEncrypted);
    }
    void doPrepare()
    {
        _inputfile.open(string("input.txt"));
        _data = _inputfile.GetData();
    }
    void _des(unsigned char* key)
    {
        des.set_key(key);
        des.vector_ecb_encode(_data, _result);
    }
    void _undes(unsigned char* key2)
    {
        des.set_key(key2);
        des.vector_ecb_decode(_data, _result);
    }
    byte* _importantval;
    void _des2(unsigned char* key, unsigned char* key2)
    {
        _des(key);
        _data = std::vector<byte>(_result);

        _importantval= _result.data();
        cout << _data.data() << '_';
        cout <<(long*)&key2;
        cout<< std::endl;
        _des(key2);
        
        //for (int i = 0; i < _inputfile.GetData().size(); i++)cout << _inputfile.GetData().at(i);
        /*cout << " ---------------------- ";
        for (int i = 0; i < _result.size(); i++)cout << _result.at(i);
        cout << std::endl;*/
        //inputfile.GetData() contains plaintext
        //result contains ciphertext
        
        //des.set_key(key2);
        //des.vector_ecb_decode(data, result);
        //des.set_key(key);
        //des.vector_ecb_decode(result, data);
        //Foper out;
        //out.GetData() = data;
        //out.write(string("res.txt"));
        _2desEncrypted = _result;
    }
};