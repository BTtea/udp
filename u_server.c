#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 512
#define SERVER_PORT 3001 
int main(int argc,char **argv){

    int nServerSockFd;
    struct sockaddr_in addrServer, addrClient;
    socklen_t nLen=sizeof(struct sockaddr);
    char cSendBuffer[BUFFER_SIZE],cRecvBuffer[BUFFER_SIZE];

    memset(&addrClient,0,sizeof(addrClient));
    memset(&addrServer,0,sizeof(addrServer));

    addrServer.sin_family=AF_INET;
    addrServer.sin_port=htons(SERVER_PORT);

    if(argc>=2){
        addrServer.sin_addr.s_addr=inet_addr(argv[1]);
    }
    else{
        addrServer.sin_addr.s_addr=inet_addr("127.0.0.1");
    }

    nServerSockFd=socket(AF_INET,SOCK_DGRAM,0);

    bind(nServerSockFd,(struct sockaddr *)&addrServer,sizeof(addrServer));

    

    for(;;){
        int nReadBytes=recvfrom(nServerSockFd,cRecvBuffer,sizeof(cRecvBuffer),0,(struct sockaddr *)&addrClient,&nLen);
    }


    return 0;
}
