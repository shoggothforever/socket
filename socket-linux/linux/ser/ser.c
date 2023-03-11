#include"../m_net.h"
char buf[BUFS*11];

void child_proc_middleware(int sig){
    pid_t pid;
    int status;
    pid=waitpid(-1,&status,WNOHANG);
    printf("the removed child proc id is %d\n",pid);
}
void setnoblock(int sock){
    int flag=fcntl(sock,F_GETFL,0);
    fcntl(sock,F_SETFL,flag | O_NONBLOCK);
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
setnoblock(sersock);
//create epoll 
int epfd,event_cnt;
struct epoll_event *ep_events,event;
epfd=epoll_create(EPOLL_SIZE);
ep_events=malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
event.events=EPOLLIN;
event.data.fd=sersock;
epoll_ctl(epfd,EPOLL_CTL_ADD,sersock,&event);

//distribute some 
printf("server start!\n");
while(1){
    event_cnt=epoll_wait(epfd,ep_events,EPOLL_SIZE,-1);
    if(event_cnt==-1)logerror("read select");
    puts("return epoll_wait");
    int i;
    for(i=0;i<=event_cnt;++i){
        if(ep_events[i].data.fd==sersock){
                clisock=accept(sersock,(struct sockaddr*)&cliaddr,&szcli);
                if(clisock==-1){
                    continue; 
                }
                else{

                    //if just add EPOLLET MODE will induce the block problem, thus requiring a setnoblockingmode
                    setnoblock(clisock);
                    event.events=EPOLLIN|EPOLLET;
                    event.data.fd=clisock;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,clisock,&event);
                    printf("new client id :%d , pid : %d\n",++idx,clisock);
                }
            }else{
                while(1){
                int esock=ep_events[i].data.fd;
                int readlen=read(esock,buf,BUFS);
                if(readlen==0){
                    epoll_ctl(epfd,EPOLL_CTL_DEL,esock,NULL);
                    close(esock);
                    printf("closed client pid:%d\n",esock);
                    break;
                }
                readlen/=BUFS;
                int len=0;
                while(len<readlen){
                    int wlen=write(esock,&buf[len],BUFS);
                    if (wlen==-1)logerror("write echo");
                    else if(wlen==0)break;
                    else len+=wlen;
                }
            }
            }
        }
    }
close(sersock);
close(epfd);
puts("server closed\n");
return 0;

}