#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "file_client.h"


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
           // printf("Waiting for command...\n");
        char input[256] = "sendfile ";
        //printf("%s,%s\n",argv[0], argv[2]);
        strcat(input, argv[2]) ;
        write(sockfd, input, sizeof(input));
        //quit or not
      //  if(!strcmp(input, "quit"))
    send_file(sockfd,argv[1]);

    //write(sockfd,DATA, sizeof(DATA));
   // while(1)
    //{

        //Read command
        //printf("Waiting for command...\n");
//        char input[100];
//        gets(input);
//        //quit or not
//        if(!strcmp(input, "quit"))
//        {
//            write(sockfd, input, sizeof(input));
//            //break;
//        }

        //send command
       // write(sockfd, input, sizeof(input));
//        char cmd[10];//seem nothing wrong even though more than 10
//        char p1[256];
//        char p2[256];
//        str_split(input, cmd,p1,p2);
//        if(!strcmp(cmd,"sendfile"))
//        {
////            char* fn = get_file_name(p1);
////            printf("%s, %d\n", fn, strlen(fn));
////            write(sockfd, fn, sizeof(fn));
//            send_file(sockfd,p1);
//            printf("sendfile done.\n");
//        }
//        else if(!strcmp(cmd,"sendfilea"))
//        {
//            printf("sendfilea is not integrated.\n");
//        }
//        else if(!strcmp(cmd,"senddir"))
//        {
//            printf("senddir is not integrated.\n");
//        }
//        else if(!strcmp(cmd,"rename"))
//        {
//
//        }
//        else if(!strcmp(cmd,"delete"))
//        {
//
//        }
//        else if(!strcmp(cmd,"stop"))
//        {
//
//        }
//        else if(!strcmp(cmd,"play"))
//        {
//
//        }
//        else if(!strcmp(cmd,"playl"))
//        {
//
//        }
//        else if(!strcmp(cmd,"playlist"))
//        {
//
//        }
//        else if(!strcmp(cmd,"playlistl"))
//        {
//
//        }
//        else
//        {
//            printf("undefined command!\n");
//            //continue;
//        }
//
//        //print response
//        int rval;
//        char rbuf[256];
//        if((rval = read(sockfd,rbuf, sizeof(rbuf))) > 0)
//        {
//            printf("Server response: %s\n\n", rbuf);
//        }


    //}

    //close(sockfd);
    return 0;
}
