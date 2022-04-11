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

    int nSockFd=0;
    int nStrLen=0;
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

    nSockFd=socket(AF_INET,SOCK_DGRAM,0);

    for(;;){
        printf("Please Input Mesage:");
        fgets(cSendBuffer,BUFFER_SIZE,stdin);

        if(strncmp(cSendBuffer,"exit",4)==0){
            break;
        }

        nStrLen=strlen(cSendBuffer);
        sendto(nSockFd,cSendBuffer,nStrLen+1,0,(struct sockaddr *) &addrServer,nLen);
        nLen=recvfrom(nSockFd,cRecvBuffer,BUFFER_SIZE,0,NULL,NULL);
        printf("Receive %d bytes\n",nLen);
        printf("Echo: %s\n",cRecvBuffer);
    }
    return 0;
}
