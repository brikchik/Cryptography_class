#include <iostream>
#include <math.h>
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
		int res = (bigrams[i] * a + b) % (N*N);
		int l1 = res / N;
		int l2 = res % N;
		l1 = alphabet[l1]; l2 = alphabet[l2];
		result[i*2] = l1;
		result[i*2+1]=l2;
	}
	result[strlen(text)] = '\0';

	//cout << '=' << bigrams[2]-bigrams[0] << '='<< bigrams[3] - bigrams[1];
	//trying to find a
	int t1 = bigrams[3] - bigrams[1];
	int t2 = bigrams[2] - bigrams[0];
	delete[] bigrams;
	return result;
}
int main()
{
	cout << encode2("i always try to help", 9, 1)<<endl;
	cout << encode2("aldwheml", 0, 0)<<endl;
	cout << encode2("csdwqbohxqfjxqesmlag", rev(9,1,26)[0], rev(9,1,26)[1]) << endl;
	//cout << encode2("soebkzhi",1,1);
	//cout << encode2("glqibicnhiwbliebkxzhixeqabpn", 0, 0)<<endl;
	system("pause");
    return 0;
}

