#include"../m_net.h"
char buf[BUFS*11];

void child_proc_middleware(int sig){
    pid_t pid;
    int status;
    pid=waitpid(-1,&status,WNOHANG);
    printf("the removed child proc id is %d\n",pid);
}
int idx=0;
pid_t pid;
int main(int argc,char *argv[]){

    //check if arguments are correct
if(argc!=2){
    printf("please input as form like:  %s   <port>\n",argv[0]);
    exit(-1);
}
//use select to realize the multiplexer
struct timeval timeout;
fd_set reads,tmps;
int fd_max,fd_num;

//set sigaction to prevent child proc from being zombie
act.sa_handler=child_proc_middleware;
__sigemptyset(&act.sa_mask);
act.sa_flags=0;

//create socket for server
int sersock,clisock;
struct sockaddr_in seraddr,cliaddr;
socklen_t szcli=sizeof(cliaddr);
sersock=socket(AF_INET,SOCK_STREAM,0);
if(sersock==-1)logerror("create serversocket");
configsockaddr(&seraddr,AF_INET,htonl(INADDR_LOOPBACK),htons(atoi(argv[1])));
int option=TRUE;
setsockopt(sersock,SOL_SOCKET,SO_REUSEADDR,(void*)&option,sizeof option);
//bind and listen
// printf("%d %d\n",seraddr.sin_addr.s_addr,seraddr.sin_port);
int state=bind(sersock,(struct sockaddr *)&seraddr,sizeof (seraddr));
if(state==-1)logerror("bind server sokcet");
state=listen(sersock,10);
if(state==-1)logerror("server listen");

FD_ZERO(&reads);
FD_SET(sersock,&reads);
fd_max=sersock;

//distribute some 
printf("server start!\n");
while(1){
    fd_set cpy_reads=reads;
    timeout.tv_sec=5;
    timeout.tv_usec=5000;
    if((fd_num=select(fd_max+1,&cpy_reads,0,0,&timeout))==-1)logerror("read select");
    else if(fd_num==0)continue;
    int i;
    for(i=0;i<=fd_max;++i){
        if(FD_ISSET(i,&cpy_reads)){
            if(i==sersock){
                clisock=accept(sersock,(struct sockaddr*)&cliaddr,&szcli);
                if(clisock==-1){
                    continue; 
                }
                else{
                    printf("new client id :%d , pid : %d\n",++idx,clisock);
                }
                //SET new fd_set
                FD_SET(clisock,&reads);
                if(fd_max<clisock)fd_max=clisock;                
            }else{
                int readlen=read(i,buf,BUFS);
                if(readlen==0){
                    FD_CLR(i,&reads);
                    close(i);
                    printf("closed client pid:%d\n",i);
                    continue;
                }
                readlen/=BUFS;
                int len=0;
                while(len<readlen){
                    int wlen=write(i,buf,BUFS);
                    if (wlen==-1)logerror("write echo");
                    else if(wlen==0)break;
                    else len+=wlen;
                }
            }
        }
    }
}
close(sersock);
puts("server closed\n");

return 0;

}