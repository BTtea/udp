#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *serverthread(void *parm);

pthread_mutex_t mut;

#define SERVERPORT 2140
#define QLEN 10

int client_counter = 0;

int main(int argc, char **argv)
{
    struct hostent *phost;
    struct protoent *pprotocol;

    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;

    int sockfd, sockfd2;

    int port, alen;

    pthread_t tid;

    pthread_mutex_init(&mut, NULL);

    memset((char *)&serveraddr, 0, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    if (argc > 1)
        port = atoi(argv[1]);
    else
        port = SERVERPORT;

    if (port > 0)
        serveraddr.sin_port = htons((u_short)port);
    else
    {
        fprintf(stderr, "bad port number %s\n", argv[1]);
        // fprintf(stderr,"cannot map \"tcp\" to protocol number");
        exit(1);
    }

    if (((long)(pprotocol = getprotobyname("tcp"))) == 0)
    {
        fprintf(stderr, "cannot map \"tcp\" to protocol number");
        exit(1);
    }

    sockfd = socket(PF_INET, SOCK_STREAM, pprotocol->p_proto);
    if (sockfd < 0)
    {
        fprintf(stderr, "socket creation failed\n");
        exit(1);
    }

    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        fprintf(stderr, "bind failed\n");
        exit(1);
    }

    if (listen(sockfd, QLEN) < 0)
    {
        fprintf(stderr, "lsiten failed");
        exit(1);
    }

    alen = sizeof(clientaddr);

    while (1)
    {
        printf("SERVER:Waiting for clients...\n");

        if ((sockfd2 = accept(sockfd, (struct sockaddr *)&clientaddr, &alen)) < 0)
        {
            fprintf(stderr, "accept failed\n");
            exit(1);
        }

        pthread_create(&tid, NULL, serverthread, (void *)&sockfd2);
    }
    close(sockfd);
    return 0;
}

void * serverthread(void * parm)
{
    int tsockfd, tclient_counter;
    char buffer[100];

    tsockfd = *((int*)parm);
    pthread_mutex_lock(&mut);
    tclient_counter = ++client_counter;
    pthread_mutex_unlock(&mut);
    sprintf(buffer, "This server has been contacted %d time%s\n", tclient_counter, tclient_counter == 1 ? "." : "s.");
    printf("SERVER thread: %s", buffer);

    send(tsockfd, buffer, strlen(buffer), 0);

    close(tsockfd);
    pthread_exit(0);
}
