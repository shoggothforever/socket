#include"../m_net.h"
char buf[BUFS];

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

//set sigaction to prevent child proc from being zombie
act.sa_handler=child_proc_middleware;
__sigemptyset(&act.sa_mask);
act.sa_flags=0;

//initsigaction(&act,child_proc_middleware);



//create socket for server
int sersock,clisock;
struct sockaddr_in seraddr,cliaddr;
socklen_t szcli=sizeof(cliaddr);
sersock=socket(AF_INET,SOCK_STREAM,0);
if(sersock==-1)logerror("create serversocket");
configsockaddr(&seraddr,AF_INET,htonl(INADDR_LOOPBACK),htons(atoi(argv[1])));
//bind and listen
// printf("%d %d\n",seraddr.sin_addr.s_addr,seraddr.sin_port);
int state=bind(sersock,(struct sockaddr *)&seraddr,sizeof (seraddr));
if(state==-1)logerror("bind server sokcet");
state=listen(sersock,10);
if(state==-1)logerror("server listen");

//distribute some 
printf("server start!\n");
while(1){
clisock=accept(sersock,(struct sockaddr*)&cliaddr,&szcli);
if(clisock==-1){
 continue; 
}
else{
    printf("new client id:%d\n",idx+1);
}
++idx;
//begin child proc
pid=fork();
if(pid==-1){
    close (clisock);
    continue;
}
if (pid==0){
    printf("child proc %d start\n",idx);
    //fork function copy all socket ,it is neccessary to close sersocket firstly
    close(sersock);
    //client charges for writing;
    int len=0;
    while(len=read(clisock,buf,BUFS)!=0){
        if(len==-1)logerror("read message");
        write(clisock,buf,len);
    }
    close(clisock);
    puts("client disconnected......\n");
    return 0;
}

}
puts("server done\n");
close(sersock);
return 0;

}