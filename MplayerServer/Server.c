#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "server.h"

#define SERVPORT 4343
#define BACKLOG 10
#define MAXSIZE 256

char pa[] = "/sdcard/tony/";
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

void handler(int num)
{
    int status;
    int pid = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        printf("The child %d exit with code %d\n",pid, WIFEXITED(status));
    }
}

int main()
{

    int sockfd,client_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in remote_addr;

    //创建套接字
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket create failed!");
        exit(1);
    }

    //绑定端口地址
    my_addr.sin_family      = AF_INET;
    my_addr.sin_port        = htons(SERVPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    bzero(&(my_addr.sin_zero),8);
    int bReuseaddr = 1;
    setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR,(const char*)&bReuseaddr, sizeof(int));
    if (bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind error!");
        exit(1);
    }

    //监听端口
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen error");
        exit(1);
    }

    socklen_t sin_size = sizeof(struct sockaddr_in);

    char initplaycmd[256] = "mplayer -loop 0 -quiet -playlist /sdcard/tony/playlist";
    int c_pid, pid;
    signal(SIGCHLD, handler);
    if((pid = fork()))
    {
        //parent
        c_pid = pid;
        printf("The child process is %d\n", c_pid);
        //parent go to end
    }
    else
    {
        //child
        printf("I am a child doing play\n");
        popen(initplaycmd, "w");
        exit(0);
    }

    char ledcmd[256] = "/home/plg/led";
    int c_pid_led, pid_led;
    signal(SIGCHLD, handler);
    if((pid_led= fork()))
    {
        //parent
        c_pid_led = pid_led;
        printf("The child process is %d\n", c_pid);
        //parent go to end
    }
    else
    {
        //child
        printf("I am a child doing play\n");
        popen(ledcmd, "w");
        exit(0);
    }

    while (1)
    {
        if ((client_fd = accept(sockfd, (struct sockaddr*)&remote_addr,&sin_size)) == -1)
        {
            perror("accept error!");
            continue;
        }
        //接受client发送的请示信息
        int rval;
        char buf[MAXSIZE] ;
        if ((rval = read(client_fd, buf, MAXSIZE)) < 0)
        {
            perror("reading stream error!");
            continue;
        }
        printf("received cmd: %s|\n",buf);

        //process command
        char cmd[10];//seem nothing wrong even though more than 10
        char p1[256];
        char p2[256];
        str_split(buf, cmd,p1,p2);
        //printf("cmd:%s\np1:%s\np2:%s\n", cmd, p1, p2);
        if(!strcmp(cmd,"sendfile"))
        {
            char path[256] = "";
            strcat(path,pa);
            strcat(path,p1);
            receiveFile(client_fd, path);
        }
        else if(!strcmp(cmd,"sendfilea"))
        {
            receiveFile(client_fd, p1);
        }
        else if(!strcmp(cmd,"senddir"))
        {
            int nfile = atoi(p1);
            while(nfile--)
            {
                char path[256] = "";
                strcat(path,pa);
                strcat(path,p1);
                receiveFile(client_fd, path);
            }
            char rmcmd[256] ;
            sprintf(rmcmd, "rm %s%s",pa,p1);
            printf("test rmcmd:%s\n",rmcmd);
            system(rmcmd);
        }
        else if(!strcmp(cmd,"rename"))
        {
            char mv[256] ;
            sprintf(mv, "mv %s%s %s%s", pa, p1, pa, p2);
            system(mv);
        }
        else if(!strcmp(cmd,"deleteall"))
        {
            char deleteallcmd[256];
            sprintf(deleteallcmd,"rm -rf %s",pa);
            system(deleteallcmd);
            char mkdircmd[256];
            sprintf(mkdircmd, "mkdir %s", pa);
            system(mkdircmd);
        }
        else if(!strcmp(cmd,"stop"))
        {
            char c[256] = "killall mplayer -q";
            popen(c, "w");
            //system(c);
            printf("%s\n",c);
            //printf("cmd \"stop\" is integrated.\n");
        }
        else if(!strcmp(cmd,"play"))
        {
            char fp[256];
            sprintf(fp, "%s%s", pa, p1);
            int filecheck = open(fp,O_RDWR);
            if(filecheck != -1)
            {
                char playcmd[256];
                sprintf(playcmd, "mplayer -slave -quiet %s%s",pa,p1 );
                int c_pid, pid;
                signal(SIGCHLD, handler);
                if((pid = fork()))
                {
                    //parent
                    c_pid = pid;
                    printf("The child process is %d\n", c_pid);
                    //parent go to end
                }
                else
                {
                    //child
                    printf("I am a child doing play\n");
                    popen(playcmd, "w");
                    exit(0);
                }

                //system(playcmd);
            }
            else
            {
                printf("file not exist.\n");
            }
            close(filecheck);
        }
        else if(!strcmp(cmd,"playl"))
        {
            char fp[256];
            sprintf(fp, "%s%s", pa, p1);
            int filecheck = open(fp,O_RDWR);
            if(filecheck != -1)
            {
                char playlcmd[256];
                sprintf(playlcmd, "mplayer -loop 0 -quiet %s%s",pa,p1 );
                int c_pid, pid;
                signal(SIGCHLD, handler);
                if((pid = fork()))
                {
                    //parent
                    c_pid = pid;
                    printf("The child process is %d\n", c_pid);
                    //parent go to end
                }
                else
                {
                    //child
                    printf("I am a child doing play\n");
                    popen(playlcmd, "w");
                    exit(0);
                }
                //system(playlcmd);
            }
            else
            {
                printf("file not exist.\n");
            }
            close(filecheck);
        }
        else if(!strcmp(cmd,"playlist"))
        {
            char fp[256];
            sprintf(fp, "%s%s", pa, p1);
            int filecheck = open(fp,O_RDWR);
            if(filecheck != -1)
            {
                char playlistcmd[256];
                sprintf(playlistcmd, "mplayer -quiet -playlist %s%s",pa,p1 );
                int c_pid, pid;
                signal(SIGCHLD, handler);
                if((pid = fork()))
                {
                    //parent
                    c_pid = pid;
                    printf("The child process is %d\n", c_pid);
                    //parent go to end
                }
                else
                {
                    //child
                    printf("I am a child doing play\n");
                    popen(playlistcmd, "w");
                    exit(0);
                }
                //system(playlistcmd);
            }
            else
            {
                printf("list not exist.\n");
            }
            close(filecheck);
        }
        else if(!strcmp(cmd,"playlistl"))
        {
            char fp[256];
            sprintf(fp, "%s%s", pa, p1);
            int filecheck = open(fp,O_RDWR);
            if(filecheck != -1)
            {
                close(filecheck);
                FILE *stream;
                int flag = 0;
                char fp[256];
                sprintf(fp, "%s%s", pa, p1);
                stream = fopen(fp, "r");
                memset(fp,0,sizeof(fp));
                while(fgets(fp, 256, stream))
                {
                    fp[strlen(fp)-1] = '\0';
                    int filecheck_inner;
                    char ffpp[256];
                    sprintf(ffpp, "%s%s", pa, fp);
                    if( -1 == (filecheck_inner=  open(ffpp, O_RDWR)) )
                    {
                        flag++;
                        printf("%s:%s\n",strerror(errno), ffpp);
                    }
                    else
                    {
                        close(filecheck_inner);
                    }
                    memset(fp,0,sizeof(fp));
                }
                fclose(stream);
                if(!flag)
                {
                    char playlistlcmd[256];
                    sprintf(playlistlcmd, "mplayer -loop 0 -quiet -playlist %s%s",pa,p1 );
                    int c_pid, pid;
                    signal(SIGCHLD, handler);
                    if((pid = fork()))
                    {
                        //parent
                        c_pid = pid;
                        printf("The child process is %d\n", c_pid);
                        //parent go to end
                    }
                    else
                    {
                        //child
                        printf("I am a child doing play\n");
                        popen(playlistlcmd, "w");
                        exit(0);
                    }
                    //  system(playlistlcmd);
                }
            }
            else
            {
                printf("list not found:%s\n",fp);
            }
        }
        else
        {
            printf("undefined command!\n");
            continue;
        }

        //向client发送信息
//        char msg[64] = "command processed.";
//        write(client_fd, msg, sizeof(msg));

        printf("this round finished.\n\n");
        close(client_fd);
    }

    close(sockfd);
    return 0;
}
