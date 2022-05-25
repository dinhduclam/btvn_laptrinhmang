#include <stdio.h>
#include <winsock.h>

#pragma comment(lib, "ws2_32")

DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;

    int ret;
    char buf[256];
    char cmd[32], id[32], tmp[32];
    
    closesocket(client);
}

int main()
{
    WSAData wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKADDR_IN addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    bind(listener, (sockaddr*)&addr, sizeof(addr));

    listen(listener, 5);

    while (1) {
        SOCKET client = accept(listener, NULL, NULL);
        
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }

    closesocket(listener);
    WSACleanup();
    

}

