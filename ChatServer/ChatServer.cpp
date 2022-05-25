#include <stdio.h>
#include <winsock.h>

#pragma comment(lib, "ws2_32")

#define CONNECT "CONNECT"
#define DISCONNECT "DISCONNECT"
#define SEND "SEND"
#define LIST "LIST"
#define ERROR "ERROR"
#define MESSAGE_ALL "MESSAGE_ALL"
#define MESSAGE "MESSAGE"
#define USER_CONNECT "USER_CONNECT"
#define USER_DISCONNECT "USER_DISCONNECT"

SOCKET clients[64];
int numClient = 0;

void Connect(char* content) {

}

void Disconnect(char* content) {

}

void SendMess(char* content) {

}

void GetUsersCurrentlyLogged(char* content) {

}

void SendMessToClients(SOCKET client, char* protocol, char* mess) {
    
}

void Excute(char* req) {
    if (req == NULL || strlen(req) < 1) 
        return;

    char* protocol;
    char* content;

    for (int i = 0; i < strlen(req); i++) {
        if (req[i] == ' ') {
            req[i] = 0;
            protocol = req;
            content = req + i + 1;
            break;
        }
    }

    if (protocol == NULL || content == NULL)
        return;

    if (strcmp(protocol, "[CONNECT]") == 0){
        Connect(content);
    }
    else if (strcmp(protocol, "[DISCONNECT]") == 0) {
        Disconnect(content);
    }
    else if (strcmp(protocol, "[SEND]") == 0) {
        SendMess(content);
    }
    else if (strcmp(protocol, "[LIST]") == 0) {
        GetUsersCurrentlyLogged(content);
    }
    else {
        //sai giao thuc
    }

}


DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;

    int ret;
    char buf[256];

    while (1) {
        int ret = recv(client, buf, sizeof(buf) - 1, 0);
        if (ret < 0) {
            break;
        }
        buf[ret] = 0;
        if (buf[ret - 1] == '\n') buf[ret - 1] = 0;

        Excute(buf);
        
    }
        
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
        clients[numClient++] = client;
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }

    closesocket(listener);
    WSACleanup();
    

}

