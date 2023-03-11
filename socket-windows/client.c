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
	char message[BUFSIZ];
	while (1) {
		int len = 0, sz = 0, tmp = 0;
		puts("传达你的请求\n");
		fgets(message, sizeof message-1, stdin);
		if (strcmp(message, "bye\n") == 0)break;
		sz = send(hsocket, message,BUFSIZ, 0);
		sz /= BUFSIZ;
		Sleep(1);
		printf("发送数据%d\n", sz);
		while (len < sz) {
			tmp = recv(hsocket, &message[len*BUFSIZ],BUFSIZ, 0);
			if (tmp == -1)logerror("recv");
			len += tmp;
		}
		message[BUFSIZ] = '\0';
		printf("回应: %s\n", message);
	}

	closesocket(hsocket);
	WSACleanup();
	return 0;



}

