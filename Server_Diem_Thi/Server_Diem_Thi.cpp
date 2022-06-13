#include <stdio.h>
#include <winsock.h>
#include <string.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable : 4996)

DWORD WINAPI Func(LPVOID arg) {

	SOCKET client = *(SOCKET*)arg;

	send(client, "Please type username and password to login (mssv:<mssv> pass:<pass>): ", 71, 0);

	char buf[1024];
	float diem = 0;

	bool loginSuccess = false;

	while (1) {
		int ret = recv(client, buf, sizeof(buf) - 1, 0);

		if (ret == SOCKET_ERROR) {
			closesocket(client);
			break;
		}

		buf[ret - 1] = 0;

		char username[100], password[100];
		sscanf(buf, "%s %s", username, password);

		if (strncmp(username, "mssv:", 5) != 0 || strncmp(password, "pass:", 5) != 0) {
			send(client, "Wrong syntax (mssv:<mssv> pass:<pass>): ", 45, 0);
			continue;
		}
		
		

		FILE* f = fopen("C:\\data.txt", "r");

		while (!feof(f)) {
			char user[100], pass[100];
			float diemthi;
			fscanf(f, "%s %s %f", user, pass, &diemthi);

			if (strcmp(user, username + 5) == 0 && strcmp(pass, password + 5) == 0) {
				loginSuccess = true;
				diem = diemthi;
				break;
			}
		}

		if (loginSuccess) {
			char response[100];
			sprintf(response, "OK %.2f", diem);
			send(client, response, strlen(response), 0);
			closesocket(client);
			fclose(f);
			break;
		}
		else 
			send(client, "ERROR Login fail!\n", 19, 0);

		fclose(f);
	}
}

int main() {

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);

	bind(listener, (sockaddr*)&addr, sizeof(addr));

	listen(listener, 5);

	

	while (1) {
		SOCKET client = accept(listener, NULL, NULL);
		CreateThread(0, 0, Func, &client, 0, 0);
	}

	
}