#include <stdio.h>
#include <WinSock2.h>
#include <cstring>
#include <string>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
#pragma warning(disable:4700)

int main() {

	char target_ip[20] = "127.0.0.1";
	unsigned short target_port = 8000u;

	scanf("%s %hu", target_ip, &target_port);

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr(target_ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(target_port);

	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int ret = connect(client_socket, (SOCKADDR*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR) {
		printf("Loi ket noi\n");
		return 0;
	};

	char client_info[100] = "";

	//todo get computer name
	CHAR ComputerName[MAX_COMPUTERNAME_LENGTH];
	DWORD   size = MAX_COMPUTERNAME_LENGTH;
	if (GetComputerNameA(ComputerName, &size)) {
		strcat(client_info, "Computer Name: ");
		strcat(client_info, ComputerName);
		strcat(client_info, "\n");
	};

	//todo get drive name
	CHAR DriveName[32];
	int drive_leng = GetLogicalDriveStringsA(256, DriveName);
	if (drive_leng) {
		strcat(client_info, "Drive Name: ");
		for (int i = 0; i < drive_leng; i += 4)
			strcat(client_info, DriveName + i);
		strcat(client_info, "\n");
	};

	//todo get free space
	LPCSTR root = "";
	DWORD sectorsPerCluster;
	DWORD bytesPerSector;
	DWORD numberOfFreeClusters;
	DWORD totalNumberOfClusters;
	long long free_space = 0;

	for (int i = 0; i < drive_leng; i += 4) {
		root = DriveName + i;
		if (GetDiskFreeSpaceA(root, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters))
			free_space += (long long)bytesPerSector * sectorsPerCluster * numberOfFreeClusters;
	}
	std::string s = (std::to_string(free_space));
	const char* freespace = s.c_str();

	strcat(client_info, "Free Space: ");
	strcat(client_info, freespace);
	strcat(client_info, " Byte\n");

	//todo send client info
	send(client_socket, client_info, strlen(client_info), 0);

	closesocket(client_socket);
	WSACleanup();
}