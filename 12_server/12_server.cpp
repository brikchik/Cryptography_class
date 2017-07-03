#include "imports.h"
//http://programmerinfo.ru/cplussocket/?v=f9308c5d0596
int retVal=0;
SOCKET servSock;
SOCKET clientSock;
WSADATA wsaData;
char* keyArr = new char[50];
bool sendM(char* message, int size)
{
    retVal = send(clientSock, message, size, 0);
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to send\n");
        WSACleanup();
        return false;
    }
    else
        return true;
}
char* getM(int size)
{
    char* message = new char[size];
    retVal = recv(clientSock, message, size, 0);
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to recv\n");
        WSACleanup();
    }
    return message;
}
void init()
{
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (servSock == INVALID_SOCKET)
    {
        printf("Unable to create socket\n");
        WSACleanup();
        return;
    }
    SOCKADDR_IN sin;
    sin.sin_family = PF_INET;
    sin.sin_port = htons(508);
    sin.sin_addr.s_addr = INADDR_ANY;

    retVal = bind(servSock, (LPSOCKADDR)&sin, sizeof(sin));
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to bind\n");
        WSACleanup();
        return;
    }
    std::cout << "Waiting for connection...\n";
    retVal = listen(servSock, 10);
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to listen\n");
        WSACleanup();
        return;
    }
    clientSock = accept(servSock, NULL, NULL);
    if (clientSock == INVALID_SOCKET)
    {
        printf("Unable to accept\n");
        WSACleanup();
        return ;
    }
    printf("Connection established sucessfully\n");
}
void finish()
{
    closesocket(clientSock);
    closesocket(servSock);
    WSACleanup();
}
bool Diffie_hellman()
{
    char* Nvalue = "2156789098765496785678";
    char* Pvalue = "98765434567890987654";
    mpz_t N,P;
    mpz_init_set_str(N, Nvalue, 10);
    mpz_init_set_str(P, Pvalue, 10);
    gmp_printf("P = %Zd\nN = %Zd\n", P, N);
    mpz_t B;
    mpz_init(B);
    gmp_randstate_t r_state;
    gmp_randinit_default(r_state);
    gmp_randseed_ui(r_state, time(NULL)+3456543);
    mpz_init(B);
    mpz_urandomb(B, r_state, 80);
    gmp_printf("My number: %Zd\n", B);
    gmp_randclear(r_state);
    mpz_t Btrans;
    mpz_init(Btrans);
    mpz_powm(Btrans, P, B,N);// Atrans=P^A%N
    char key[50];
    gmp_sprintf(key, "%Zd", Btrans);
    gmp_printf("P^BmodN: %Zd\n", Btrans);
    mpz_clear(B);
    init();
    char* Atrans = getM(50);
    if (Atrans == "")return false;
    gmp_printf("Got the key from target: %s\n", Atrans);
    if (sendM(key, 50))gmp_printf("Sent key to target\n");
    mpz_t KEY; mpz_init(KEY);
    mpz_t Areceived; mpz_init(Areceived);
    mpz_set_str(Areceived, Atrans, 10);
    mpz_powm(KEY, Btrans,Areceived, N);
    gmp_printf("KEY: %Zd\n", KEY);
    for (int i = 0; i < 50; i++)keyArr[i] = 0;
    gmp_sprintf(keyArr, "%Zd", KEY);
    return true;
}
#define messageSize 50
char* rc4get()
{
    rc4 rc4class;
    rc4class.setKey(string(keyArr));
    printf("Waiting for text\n");
    char *message = getM(messageSize);
    rc4class.ms();
    for (int i = 0; i < messageSize; i++)message[i] ^= rc4class.get();
    return message;
}
int main(void)
{
    if (!Diffie_hellman())return 1;
    char* result = rc4get();
    std::cout << "Text received(50symbols): "<<result;
    printf("\nClosing connection...\n");
    finish();
    system("pause");
    return 0;
}