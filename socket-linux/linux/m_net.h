#ifndef MNET_H
#define MNET_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/select.h>
#include<sys/epoll.h>
#include<errno.h>
#include<fcntl.h>
#define EPOLL_SIZE 50
#define BUFS 512
#define TRUE 1
struct sigaction act;

void logerror(const char* seq) {
    printf("there is an error happen, which is %s function\n", seq);
    exit(-1);
}
void configsockaddr(struct sockaddr_in *seraddr,int contrast,u_int32_t addr, u_int16_t port){
memset(seraddr,0,sizeof seraddr);
seraddr->sin_addr.s_addr=addr;
seraddr->sin_family=contrast;
seraddr->sin_port=port;
return;
}
void initsigaction(struct sigaction *sact,void(*handler)(int)){
    sact->sa_handler=handler;
    __sigemptyset(&(sact->sa_mask));
    sact->sa_flags=0;
}
void createsersock(int *sersock,struct sockaddr_in* seraddr){
int state=bind(*sersock,(struct sockaddr *)seraddr,sizeof (seraddr));
if(state==-1)logerror("bind server sokcet");
state=listen(*sersock,10);
if(state==-1)logerror("server listen");
}

#endif
