#include "../shared/Foper.cpp"
#include "../shared/des.h"
class util
{
public:
    std::vector<byte> left, middle, right;
    Foper _inputfile;
    std::vector<byte> _result;
    des_class des;
    void _des(std::vector<byte> _data,unsigned char* key)
    {
        des.set_key(key);
        while ((_data.size() % 8) != 0)_data.push_back(0);
        des.vector_ecb_encode(_data, _result);
    }
    void _undes(std::vector<byte> _data,unsigned char* key2)
    {
        des.set_key(key2);
        des.vector_ecb_decode(_data, _result);
    }
    void _des2(unsigned char* key, unsigned char* key2)
    {
        _inputfile.open(string("input.txt"));
        std::vector<byte> _data = _inputfile.GetData();
        left = std::vector<byte>(_data);
        cout << "Plaintext: ";
        for (int i = 0; i < _data.size(); i++)cout << _data.data()[i];
        cout<< '\n';
        _des(_data,key);
        _data = std::vector<byte>(_result);
        middle = std::vector<byte>(_result);
        cout << "E(k1,m): " << _result.data() << '\n';
        _result.clear();
        _des(_data,key2);
        right = std::vector<byte>(_result);
        cout << "c = E(k2,E(k1,m)): " << _result.data() << '\n';
        _undes(right, key2);
        cout << "D(k2,c): " << _result.data() <<'\n';
        _undes(middle, key);
        cout << "D(k1,D(k2,c)): " << _result.data()<<'\n';
    }
};