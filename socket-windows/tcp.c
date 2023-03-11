//
// Created by 13372 on 2023/2/26.
//
#include"mnet.h"
int main(int argc, char* argv[]) {
    WSADATA wsadata;
    SOCKET hServSocket, hCliSocket;
    SOCKADDR_IN servAddr, cliAddr;
    int szCliAddr;
    const char message[] = "hello, this is client ,I want to get back my power\n";
    //检验输入参数正确性
    if (argc != 2) {
        logerror("you need input as the form like: <port>");
    }
    //启动winsocket
    if (WSAStartup(MAKEWORD(2, 2), &wsadata)) {
        logerror("WSAstart function");
    }
    else {
        printf("%d %lld\n",atoi(argv[1]),htons(atoi(argv[1])));
    }
    //获取套接字
    hServSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (hServSocket == INVALID_SOCKET) {
        logerror("server socket create");
    }
    //初始化服务器地址
    memset(&servAddr, 0, sizeof servAddr);
    servAddr.sin_family = AF_INET;//协议簇，在此处代表的是IPV4
    servAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//将主机的IP设为网络回环地址，使用htonl将主机序（amd或intel通常都是小端序）转化为网络序（大端序）转化为unsigned long long 类型
    servAddr.sin_port = htons(atoi(argv[1]));//获取端口号，不过要使用htons

    printf("hosts ip:%s,port:%d\n", inet_ntoa(servAddr.sin_addr), servAddr.sin_port);
    //向套接字分配地址
    if (bind(hServSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        logerror("bind");
    }

    szCliAddr = sizeof(cliAddr);
    //服务器持续监听客户端请求并开放套接字给客户端
    if (listen(hServSocket, 5) == SOCKET_ERROR) {
        logerror("listen");
    }
    int idx = 0;
    char echo[BUFSIZ] = {'\0'};
    while (1) {
        hCliSocket = accept(hServSocket, (SOCKADDR*)&cliAddr, &szCliAddr);//生成套接字，若无client请求会阻塞
        if (hCliSocket == INVALID_SOCKET) {
            logerror("client socket create");
        }
        else {
            printf("the connected client id is %d\n", ++idx);
        }
        int echolen;
        while (echolen = recv(hCliSocket, echo, BUFSIZ, 0) != 0) {
            if (echolen == -1)logerror("server recv");
            printf("收到消息:%s,长度为%d\n", echo,echolen*BUFSIZ);
            send(hCliSocket, echo, BUFSIZ, 0);
        }
        closesocket(hCliSocket);
    }
    closesocket(hServSocket);

    WSACleanup();
    return 0;
}