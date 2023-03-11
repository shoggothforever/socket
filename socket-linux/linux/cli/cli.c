#include"../m_net.h"
socklen_t clisz;
int sersock;
struct sockaddr_in seraddr;
char message[BUFS*11];
int main(int argc,char* argv[]){
    if(argc!=3){
        printf("please input as form: %s <ip> <port>\n",argv[0]);
        exit(-1);
    }
    sersock=socket(AF_INET,SOCK_STREAM,0);
    if(sersock==-1)logerror("create cli socket ");
    seraddr.sin_addr.s_addr=inet_addr(argv[1]);
    seraddr.sin_family=AF_INET;
    seraddr.sin_port=htons(atoi(argv[2]));
    if(connect(sersock,(struct sockaddr*)&seraddr,sizeof(seraddr))==-1){
        logerror("connect server");
    }else{
        printf("connected server socket : %d\n",sersock);
    }
    while(1){
    fputs("input some words,and you will receive reply,input q or Q to exit\n",stdout);
    fgets(message,BUFS,stdin);
    if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))break;
    int strlen=write(sersock,message,BUFS);
    printf("strlen:%d\n",strlen);
    int len=0;
    while(len<strlen){
        int recv_len=read(sersock,&message[len*BUFS],BUFS);
        printf("recvlen:%d\n",recv_len);
        if(recv_len==-1){
            logerror("read echo");
        }
        len+=recv_len;
    }
    // message[len]='\0';
    printf("echo: %s\n",message);
    // fgetc(stdin);
    }

}