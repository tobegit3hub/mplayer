#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "file_client.h"
#include <dirent.h>
#define MAX_LEN 256

#define SERVPORT 4343
#define MAXDATASIZE 100
#define SERVER_IP "192.168.1.64"
#define DATA  "I love Linux..."
void str_split(char commdand[], char cmd[], char p1[], char p2[])
{
    char* tmp = commdand;
    int cnt = 0;
    while(*tmp)
    {
        if(' ' == *tmp)
            cnt++;
        tmp++;
    }
    //printf("para: %d\n", cnt+1);
    if(!cnt)
    {
        strcpy(cmd, commdand);
        return;
    }
    char* p;
    p = strtok(commdand, " ");
    strcpy(cmd,p);
    p = strtok(NULL, " ");
    strcpy(p1, p);
    if(cnt > 1)
    {
        p = strtok(NULL, " ");
        strcpy(p2,p);
    }
}

//get file_name
char* get_file_name(char path[])
{
    char* tmp = strrchr(path, '/');
    char* result;
    return strcpy(result, tmp+1);
}

int main(int argc, char* argv[])
{
    int sockfd, recvbytes;
    char buf[MAXDATASIZE];
    struct hostent *host;
    struct sockaddr_in serv_addr;

    if (( sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error!");
        exit(1);
    }
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family    = AF_INET;
    serv_addr.sin_port      = htons(SERVPORT);
    serv_addr.sin_addr.s_addr= inet_addr(SERVER_IP);


    if (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("connect error!");
        exit(1);
    }

    char input[256] ;
    sprintf(input,"playl %s", argv[1]);
    write(sockfd, input, sizeof(input));

}
