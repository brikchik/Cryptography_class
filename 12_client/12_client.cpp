#pragma once
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <iostream>
#include "../SHARED/rc4.h"
#include "../THIRD-PARTY/src/mpir-3.0.0/lib/Win32/Debug/mpir.h"
#pragma comment (lib, "../third-party/lib/mpir.lib")
#pragma comment (lib, "Ws2_32.lib")
//http://programmerinfo.ru/cplussocket/?v=f9308c5d0596
int retVal = 0;
SOCKET clientSock;
char* keyArr = new char[50];
int init()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA wsaData;
    WSAStartup(ver, (LPWSADATA)&wsaData);
    LPHOSTENT hostEnt;
    hostEnt = gethostbyname("localhost");
    clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSock == SOCKET_ERROR)
    {
        printf("Unable to create socket\n");
        WSACleanup();
        return 1;
    }
    SOCKADDR_IN serverInfo;
    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr = *((LPIN_ADDR)*hostEnt->h_addr_list);
    serverInfo.sin_port = htons(508);
    retVal = connect(clientSock, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to connect\n");
        WSACleanup();
        return 1;
    }
    printf("Connection established sucessfully\n");
}
void finish()
{
    closesocket(clientSock);
    WSACleanup();
}
bool sendM(char message[], int size)
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
    char* message=new char[size];
    retVal = recv(clientSock, message, size, 0);
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to recv\n");
        WSACleanup();
    }
    else
        return message;
}
bool Diffie_hellman()
{
    char* Nvalue = "2156789098765496785678";
    char* Pvalue = "98765434567890987654";
    mpz_t N, P;
    mpz_init_set_str(N, Nvalue, 10);
    mpz_init_set_str(P, Pvalue, 10);
    gmp_printf("P = %Zd\nN = %Zd\n", P, N);
    mpz_t A;
    mpz_init(A);
    gmp_randstate_t r_state;
    gmp_randinit_default(r_state);
    gmp_randseed_ui(r_state, time(NULL));
    mpz_init(A);
    mpz_urandomb(A, r_state, 80);
    gmp_printf("My number: %Zd\n", A);
    gmp_randclear(r_state);
    mpz_t Atrans;
    mpz_init(Atrans);
    mpz_powm(Atrans, P, A, N);// Atrans=P^A%N
    char key[50];
    gmp_sprintf(key, "%Zd", Atrans);
    gmp_printf("P^AmodN: %Zd\n", Atrans);
    mpz_clear(A);
    if (sendM(key, 50))gmp_printf("Sent key to target\n"); else return false;
    char* Btrans = getM(50);
    if (Btrans == "")return false;
    gmp_printf("Got the key from target: %s\n", Btrans);
    mpz_t KEY; mpz_init(KEY);
    mpz_t Breceived; mpz_init(Breceived);
    mpz_set_str(Breceived, Btrans, 10);
    mpz_powm(KEY, Breceived, A, N);
    gmp_printf("KEY: %Zd\n", KEY);
    for (int i = 0; i < 50; i++)keyArr[i] = 0;
    gmp_sprintf(keyArr, "%Zd",KEY);
    return true;
}
#define messageSize 50
bool rc4send()
{
    rc4 rc4class;
    rc4class.setKey(string(keyArr));
    printf("Ready to send text. Text to send: ");
    string plaintext;
    std::cin >> plaintext;
    char message[messageSize];
    rc4class.ms();
    for (int i = 0; i < messageSize; i++)message[i] =plaintext[i] ^ rc4class.get();
    sendM(message, messageSize);
    return true;
}
int main()
{
    init();
    if (!Diffie_hellman())return 1;
    rc4send();
    printf("Closing connection...\n");
    finish();
    system("pause");
    return 0;
}