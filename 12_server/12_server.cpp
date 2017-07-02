#pragma comment (lib, "Ws2_32.lib")
#include <time.h>
#include "../THIRD-PARTY/src/mpir-3.0.0/lib/Win32/Debug/mpir.h"
#pragma comment (lib, "../third-party/src/mpir-3.0.0/lib/Win32/Debug/mpir.lib")
#include <winsock2.h>
#include <stdio.h>
//http://programmerinfo.ru/cplussocket/?v=f9308c5d0596
int retVal=0;
SOCKET servSock;
SOCKET clientSock;
WSADATA wsaData;
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
}
void finish()
{
    closesocket(clientSock);
    closesocket(servSock);
    WSACleanup();
}
bool Diffie_hellman()
{
    char* Nvalue = "823122826709839759871747";
    char* Pvalue = "725115387097598744838427";
    mpz_t N,P;
    mpz_init_set_str(N, Nvalue, strlen(Nvalue));
    mpz_init_set_str(P, Pvalue, strlen(Pvalue));

    mpz_t B;
    mpz_init(B);
    gmp_randstate_t r_state;
    gmp_randinit_default(r_state);
    gmp_randseed_ui(r_state, time(NULL)-1);
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
    mpz_t KEY;
    mpz_init_set_str(KEY, Atrans, 25);
    //gmp_printf("%Zd\n",KEY);
    //mpz_add(KEY, B, KEY);
    gmp_printf("P^(AB)modN: %Zd\n", KEY);
    finish();
    return true;
}
int main(void)
{
    if (!Diffie_hellman())return 1;
    return 0;
}