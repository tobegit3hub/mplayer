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
    DIR *dp;
    struct dirent *entry, *cnt;
    struct stat statbuf;
    char dir[256];
    strcpy(dir, argv[1]);

    if (NULL == (dp = opendir(dir))) {
        fprintf(stderr, "Open dictionary fail: %s\n", dir);
        return ;
    }

    chdir(dir);

    int nfile = 0;
    while (NULL != (cnt = readdir(dp))) {
        if(strcmp(".",cnt->d_name) == 0
            || strcmp("..",cnt->d_name) == 0
            || ('.' == cnt->d_name[0]))
            continue;
        nfile++;
    }

    char f_name[nfile][MAX_LEN];
    int i, j;
    for (i = 0; i < nfile; i++) {
        for (j = 0; j < MAX_LEN; j++) {
            f_name[i][j] = '\0';
        }
    }

    if (NULL == (dp = opendir(dir))) {
        fprintf(stderr, "Open dictionary fail: %s\n", dir);
        return ;
    }
    i = 0;
    while ((NULL != (entry = readdir(dp))) && (i < nfile)) {
        if(strcmp(".",entry->d_name) == 0
            || strcmp("..",entry->d_name) == 0
            || ('.' == entry->d_name[0]))
            continue;
        strcpy(f_name[i++], entry->d_name);
        //printf("%s\n",entry->d_name);
    }
    /*for (i = 0; i < nfile; i++) {
        for (j = 0; j < MAX_LEN; j++) {
            printf("%c", f_name[i][j]);
        }
        printf("\n");
    }*/
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
    sprintf(input, "senddir %d", nfile);
    //printf("Test input: %s", input);
    write(sockfd, input, sizeof(input));

    for (i = 0; i < nfile; i++) {
        char full_path[256] = "";
        strcat(full_path, dir);
        strcat(full_path,"/");
        strcat(full_path, f_name[i]);
        printf("sending: %s\n", full_path);
        char cmd[300] = "/home/tony/res/sendfile ";
        strcat(cmd, full_path);
        strcat(cmd, " ");
        strcat(cmd,  f_name[i]);
        system(cmd);
        }
    chdir("..");
    closedir(dp);
}
