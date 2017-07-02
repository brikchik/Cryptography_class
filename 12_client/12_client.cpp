#pragma once
#include <time.h>
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include "../THIRD-PARTY/src/mpir-3.0.0/lib/Win32/Debug/mpir.h"
#pragma comment (lib, "../third-party/src/mpir-3.0.0/lib/Win32/Debug/mpir.lib")
#pragma comment (lib, "Ws2_32.lib")
//http://programmerinfo.ru/cplussocket/?v=f9308c5d0596
int retVal = 0;
SOCKET clientSock;
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
    printf("Connection made sucessfully\n");
}
void finish()
{
    closesocket(clientSock);
    WSACleanup();
}
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
    char* message=new char[size];
    retVal = recv(clientSock, message, size, 0);
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to recv\n");
        WSACleanup();
        return message;
    }
    else
        return message;
}
bool Diffie_hellman()
{
    mpz_t A;
    mpz_init(A);
    gmp_randstate_t r_state;
    gmp_randinit_default(r_state);
    gmp_randseed_ui(r_state, time(NULL));
    mpz_init(A);
    mpz_urandomb(A, r_state, 80);
    gmp_printf("My number: %Zd\n", A);
    gmp_randclear(r_state);
    mpz_clear(A);
    init();
    printf("Sending request from client\n");
    char pBuf[1] = { 'k' };
    sendM(pBuf, 25);
    char* szResponse = getM(25);
    printf("Got the response from server\n%s\n", szResponse);
    finish();
    return true;
}
int main()
{
    if (!Diffie_hellman())return 1;
    system("pause");
    return 0;
}