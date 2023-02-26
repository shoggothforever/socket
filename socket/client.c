#include"mnet.h"

int main(int argc,char *argv[] ) {
	WSADATA wsaData;
	SOCKET hsocket;
	SOCKADDR_IN servAddr;
	if (argc != 3) {
		logerror("you need input as the form like: <ip> <port>");
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		logerror("WSAstart function");
	}
	hsocket = socket(AF_INET, SOCK_STREAM, 0);
	if (hsocket == INVALID_SOCKET)logerror("create client socket");
	memset(&servAddr, 0, sizeof servAddr);
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hsocket, (SOCKADDR*)&servAddr, sizeof servAddr) == SOCKET_ERROR) {
		logerror("connect to server");
	}
	char message[1001];
	int strlen;
	strlen = recv(hsocket, message, sizeof(message) - 1, 0);
	if (strlen == -1)logerror("recv");

	printf("receive message from server %s", message);

	closesocket(hsocket);
	WSACleanup();
	return 0;



}

