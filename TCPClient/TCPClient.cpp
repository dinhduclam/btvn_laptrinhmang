#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main(int argc, char* argv[])
{
    char* target_ip = argv[1];
    unsigned short target_port = 0;

    for (int i = 0; argv[2][i]; i++) {
        target_port = target_port * 10 + argv[2][i] - '0';
    }

    printf("%s %hu\n", target_ip, target_port);

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(target_ip);
    addr.sin_port = htons(target_port);

    system("pause");

    int ret = connect(client_sock, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Connection fail - %d\n", ret);
        return 1;
    }

    char buf[256];

    ret = recv(client_sock, buf, sizeof(buf) - 1, 0);
    if (ret > 0) {
        buf[ret] = 0;
        printf("%s\n", buf);
    }

    while (1)
    {
        fgets(buf, sizeof(buf), stdin);

        if (strncmp(buf, "exit", 4) == 0) {
            printf("End. Close connection!");
            break;
        }

        send(client_sock, buf, strlen(buf), 0);
    }

    closesocket(client_sock);
    WSACleanup();
}