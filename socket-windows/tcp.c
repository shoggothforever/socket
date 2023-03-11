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
    //�������������ȷ��
    if (argc != 2) {
        logerror("you need input as the form like: <port>");
    }
    //����winsocket
    if (WSAStartup(MAKEWORD(2, 2), &wsadata)) {
        logerror("WSAstart function");
    }
    else {
        printf("%d %lld\n",atoi(argv[1]),htons(atoi(argv[1])));
    }
    //��ȡ�׽���
    hServSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (hServSocket == INVALID_SOCKET) {
        logerror("server socket create");
    }
    //��ʼ����������ַ
    memset(&servAddr, 0, sizeof servAddr);
    servAddr.sin_family = AF_INET;//Э��أ��ڴ˴��������IPV4
    servAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//��������IP��Ϊ����ػ���ַ��ʹ��htonl��������amd��intelͨ������С����ת��Ϊ�����򣨴����ת��Ϊunsigned long long ����
    servAddr.sin_port = htons(atoi(argv[1]));//��ȡ�˿ںţ�����Ҫʹ��htons

    printf("hosts ip:%s,port:%d\n", inet_ntoa(servAddr.sin_addr), servAddr.sin_port);
    //���׽��ַ����ַ
    if (bind(hServSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        logerror("bind");
    }

    szCliAddr = sizeof(cliAddr);
    //���������������ͻ������󲢿����׽��ָ��ͻ���
    if (listen(hServSocket, 5) == SOCKET_ERROR) {
        logerror("listen");
    }
    int idx = 0;
    char echo[BUFSIZ] = {'\0'};
    while (1) {
        hCliSocket = accept(hServSocket, (SOCKADDR*)&cliAddr, &szCliAddr);//�����׽��֣�����client���������
        if (hCliSocket == INVALID_SOCKET) {
            logerror("client socket create");
        }
        else {
            printf("the connected client id is %d\n", ++idx);
        }
        int echolen;
        while (echolen = recv(hCliSocket, echo, BUFSIZ, 0) != 0) {
            if (echolen == -1)logerror("server recv");
            printf("�յ���Ϣ:%s,����Ϊ%d\n", echo,echolen*BUFSIZ);
            send(hCliSocket, echo, BUFSIZ, 0);
        }
        closesocket(hCliSocket);
    }
    closesocket(hServSocket);

    WSACleanup();
    return 0;
}