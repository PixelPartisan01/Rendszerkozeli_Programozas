#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <omp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <linux/limits.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define BUFSIZE 1024 
#define PORT_NO 80
#define NEPTUN "HJTBVA"

int BrowseForOpen();
char* ReadPixels(int f, int* NumCh);
char* Unwrap(char* Pbuff, int NumCh);
void Translate (char *titkos_uzenet, int NumCh);
void Post(char *NeptunId , char *message, int NumCh);
void WhatToDo(int sig);
void ctrlc();
void stop();

int BrowseForOpen()
{
    system("clear");

    FILE* file;
    DIR* d;
    char keres[1000] = "";
    char konyvtar[1001] = ".";
    char elozo[1000] = "";
    struct dirent* entry;
    int tmp;
    struct stat inode;
    

    while (1)
    {
        d = opendir(konyvtar);

        if (!d)
        {
            tmp = stat(konyvtar, &inode);

            if (inode.st_mode & S_IFREG && strstr(konyvtar, ".bmp"))
            {
                system("clear");
                file = fopen(konyvtar, "rb");
                int fn = fileno(file);
                return fn;
            }
            //else

            strcpy(konyvtar, elozo);
            system("clear");
        }
        else
        {
            while ((entry = readdir(d)) != NULL)
            {
                if ((*entry).d_name[0] != '.')
                    printf("%s\n", (*entry).d_name);
            }

            closedir(d);

            printf("\n\nSearch: ");
            scanf("%s", keres);

            tmp = stat(keres, &inode);

            if (inode.st_mode & S_IFREG && strstr(keres, ".bmp"))
            {
                system("clear");
                file = fopen(keres, "rb");
                int fn = fileno(file);
                return fn;
            }

            strcpy(elozo, konyvtar);

            strcat(konyvtar, "/");
            strcat(konyvtar, keres);

            system("clear");

        }
    }
}

void Translate(char* titkos_uzenet, int NumCh)
{
    int z = 0;

    char resz_string[9];

    char uzenet[NumCh + 1];

    uzenet[NumCh] = '\0';

    for (int i = 0; i < NumCh; i++)
    {
        memcpy(resz_string, &titkos_uzenet[z], 8); // Rész string
        resz_string[8] = '\0';
        char c = strtol(resz_string, 0, 2); // rész string (bináris kód) átalakítása karakterré
        uzenet[i] = c;
        z += 8;
    }

    alarm(0);

    Post(NEPTUN, uzenet, NumCh);
}

char* ReadPixels(int f, int* NumCh)
{
    char* buffer;
    long length;
    

    FILE* file = fdopen(f, "rb");
    WhatToDo(SIGALRM);
    WhatToDo(SIGINT);
    alarm(1);

    if (!file)
    {
        printf("Failed to open file!\n");
        exit(1);
    }
    else
    {
        fseek(file, 0, SEEK_END);
        length = ftell(file);

        fseek(file, 6, SEEK_SET);
        fread(NumCh, 4, 1, file);

        buffer = malloc((length - 54) * 8);

        if (buffer == NULL)
        {
            printf("Memory is not allocated!\n");
            exit(1);
        }

        fseek(file, 54, SEEK_SET);
        fread(buffer, length - 54, 1, file);

        char* ff = malloc(1);
        char fff;
        int buffer_index = 0;

        for (int j = 0; j < length - 54; j++)
        {
            fseek(file, 54 + j, SEEK_SET);
            fread(ff, 1, 1, file);

            char fff = *ff;

            for (int i = 7; i >= 0; --i)
            {
                if (fff & (1 << i))
                {
                    buffer[buffer_index] = '1';
                    buffer_index++;
                }
                else
                {
                    buffer[buffer_index] = '0';
                    buffer_index++;
                }

            }
        }

        fclose(file);

        return buffer;
    }
}

char* Unwrap(char* Pbuff, int NumCh)
{
    char* kod = (char*)malloc(NumCh * 8);
    if (kod == NULL)
    {
        printf("Memory is not allocated!\n");
        exit(1);
    }

    int kodIndex = 0;
    int Pbuff_index = 0;
    char reszlet[8];
    int rgb = 1;


#pragma omp parallel for
    for (int i = 0; i < 24 * NumCh; i += 8)
    {
#pragma omp critical

        for (int i = 0; i < 8; i++)
        {
            reszlet[i] = Pbuff[Pbuff_index + i];
        }

        Pbuff_index += 8;

        if (rgb == 1)
        {
            kod[kodIndex] = reszlet[6];
            kodIndex++;
            kod[kodIndex] = reszlet[7];
            kodIndex++;

            rgb = 2;
        }
        else if (rgb == 2)
        {
            kod[kodIndex] = reszlet[5];
            kodIndex++;
            kod[kodIndex] = reszlet[6];
            kodIndex++;
            kod[kodIndex] = reszlet[7];
            kodIndex++;

            rgb = 3;
        }
        else if (rgb == 3)
        {
            kod[kodIndex] = reszlet[5];
            kodIndex++;
            kod[kodIndex] = reszlet[6];
            kodIndex++;
            kod[kodIndex] = reszlet[7];
            kodIndex++;

            rgb = 1;
        }

    }

    free(Pbuff);

    return kod;
}

void Post(char* NeptunId, char* message, int NumCh)
{
    int s;                            // socket ID
    int bytes;                        // received/sent bytes
    int err;                          // error code
    int flag;                         // transmission flag
    char on;                          // sockopt option
    char buffer[BUFSIZE];             // datagram buffer area
    unsigned int server_size;         // length of the sockaddr_in server
    struct sockaddr_in server;        // address of server
    char server_reply[2000];
    int recv_size;

    on = 1;
    flag = 0;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("193.6.135.162");
    server.sin_port = htons(PORT_NO);
    server_size = sizeof server;

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0)
    {
        printf("Socket creation error!\n");
        exit(2);
    }

    printf("\nSocket created.\n\n");

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    err = connect(s, (struct sockaddr*)&server, server_size);

    if (err < 0)
    {
        printf("Connecting error!\n");
        exit(3);
    }

    printf("Connected.\n\n");

    sprintf(buffer, "POST /~vargai/post.php HTTP/1.1\nHost: irh.inf.unideb.hu\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nNeptunID=%s&PostedText=%s", (NumCh + 27), NeptunId, message);

    bytes = send(s, buffer, strlen(buffer) + 1, flag);

    if (bytes <= 0)
    {
        printf("Sending error!\n");
        exit(4);
    }

    puts("Data Send.\n");

    if (recv(s, server_reply, 2000, 0) < 0)
    {
        puts("Recv failed!\n");
        exit(5);
    }
    else if (strstr(server_reply, "The message has been received.") == NULL)
    {
        puts("Server Reply Error!\n\n");
        puts(server_reply);
        printf("\n");
        exit(6);
    }

    printf("Server's (%s:%d) acknowledgement:\n  %s\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port), buffer);

    puts("Server Reply:\n\n");
    puts(server_reply);

    close(s);
}

void WhatToDo(int sig)
{
    if(sig == SIGALRM)
        signal(sig, stop);

    if(sig == SIGINT)
        signal(sig, ctrlc);        
}

void stop()
{
    fprintf(stderr, "\nProgram takes too long to run!\n");
    exit(7);
}

void ctrlc()
{
    pid_t pid;
    pid = fork();

    if (pid != 0)
    {
        fprintf(stderr, "\nCTRL+C won't kill the program!\n");
        kill(getpid(), SIGKILL);
    }
}