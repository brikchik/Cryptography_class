#include <iostream>
#include <string>
#include <vector>
using namespace std;
int gcd(int a, int b, int & x, int & y) {//recursive evklid algorithm
	if (a == 0) {
		x = 0; y = 1;
		return b;
	}
	int x1, y1;
	int d = gcd(b%a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}
int* rev(int a, int b, int N) //gives a^-1 and b^-1 mod N*N
{ //definitely not the best solution
	int r[2];
	int k, l;
	gcd(a, N * N, k, l);
	if (k < 0)k += N * N;
	r[0] = k;
	if (-b*k < 0)k -= N * N;
	r[1] = -b*k;
	return r;
}
char* alphabet = "abcdefghijklmnopqrstuvwxy ";
char* encode2(char* source, int a, int b)//affine mapping. Encodes text with 2-grams. iz_al_wa_ys->cs_dw_qb_oh with a=9, b=1
{
	cout << "a = " << a << " b = " << b << " ";
	int N = strlen(alphabet);
	char* text = new char[strlen(source)];
	if (strlen(text) % 2 == 1) { cout << "incorrect size, z added"; text += 'z'; }
	cout << "Text: " << source << " -> ";
	for (int i = 0;i < strlen(text);i++)if (source[i] == ' ')text[i] = 'z'; else text[i] = source[i];
	int* bigrams = new int[(strlen(text)+1)/2];
	char* result=new char[strlen(text)];
	for (int i = 0;i < strlen(text)/2;i++)
	{
		bigrams[i] = (text[i*2]-'a')*N+(text[i*2+1]-'a')<<' ';		
	}
	for (int i = 0;i < strlen(text) / 2;i++)
	{
		int res = (bigrams[i] * a + b) % (N*N);
		int l1 = res / N;
		int l2 = res % N;
		l1 = alphabet[l1]; l2 = alphabet[l2];
		result[i * 2] = l1;
		result[i * 2 + 1] = l2;
	}
	result[strlen(text)] = '\0';
	delete[] bigrams;
	return result;
}
int* getKey(char* source, int N, bool printMessage=true)//get key from 2 pairs P->C. Takes 8byte string 'aabbccdd'
{
	int bigrams [4];
	for (int i = 0;i < 4;i++)
	{
		bigrams[i] = (source[i * 2] - 'a')*N + (source[i * 2 + 1] - 'a') << ' ';
	}
	//trying to find a
	int t1 = bigrams[3] - bigrams[1];
	int t2 = bigrams[2] - bigrams[0];
	int aa = t2*rev(t1, 0, N)[0] % (N*N);
	if (aa < 0)aa += N*N;
	//trying to find b
	int bb = (bigrams[0] - bigrams[1] * aa) % (N*N);
	if (bb < 0)bb += N*N;
	if(printMessage)cout << "getting key from " << source[0] << source[1] << "->" << source[2] << source[3] 
		<< " and " << source[4] << source[5] << "->" << source[6] << source[7] << endl;
	int res[2];
	res[0] = aa;
	res[1] = bb;
	return res;
}
void Matrix(string source,bool encode=true)
{
	int m1[2][2]={ { 7,7 },{ 6,5 } };//matrix 11
	int m1D[2][2] = { {3,1},{12,25} };//reversed matrix 11
	int m2[2];//result of 2gram encoding
	int N = 2;
	string res;
	int length = source.length();
	if (length % 2 == 1) {
		cout << "incorrect size, z added\n"; source.push_back('z'); length++;
	}
	for (int i = 0;i < length;i++)source.at(i) = tolower(source.at(i));
	cout << "Text: " << source.c_str() << " -> ";
	for (int i = 0;i < length;i++)if (source[i] == ' ')source[i] = 'z';
	vector<int> grams;
	for (int i = 0;i < length;i++)
	{
		grams.push_back((source[i] - 'a')%26);
	}
	for (int c = 0;c < length;c += N)
	{
		for (int i = 0;i < N;i++)
		{//rows in 1
			if (encode)m2[i] = (m1[i][0] * grams.at(c) + m1[i][1] * grams.at(c + 1)) % 26;
			else m2[i] = (m1D[i][0] * grams.at(c) + m1D[i][1] * grams.at(c + 1)) % 26;
			if (m2[i] == 'z' - 'a')m2[i] = ' ' - 'a';
			res.push_back(m2[i]);
		}
	}
	for (int i = 0;i < length;i++)cout << (char)(res.at(i)+'a');
	cout << endl;
}
int main()
{
	string source = "Stab attack medical student spared jail";
	string source2 = "jlhftugbokldio gomsj vffplwfxbphxneneosj";
	getline(cin, source);
	Matrix(source);
	getline(cin, source2);
	Matrix(source2,false);
	//cout << encode2("i always try to help", 9, 1)<<endl;
	//cout << encode2("csdwqbohxqfjxqesmlag", getKey("aldwheml", 26)[0], getKey("aldwheml", 26,false)[1]) << endl;
	//// some tests
	//cout << encode2("glqibicnhiwbliebkxzhixeqabpn", getKey("soebkzhi", 26)[0], getKey("soebkzhi", 26, false)[1])<<endl;
	//cout << encode2("vwzwyifhpfcpaesszybxih", getKey("atcpouss", 26)[0], getKey("atcpouss", 26, false)[1]) << endl;
	system("pause");
    return 0;
}

