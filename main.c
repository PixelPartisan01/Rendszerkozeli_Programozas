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
#include "headers/SecretMessage.h"

//int BrowseForOpen();
//char* ReadPixels(int f, int* NumCh);
//char* Unwrap(char* Pbuff, int NumCh);
//void Translate (char *titkos_uzenet, int NumCh);
//void Post(char *NeptunId , char *message, int NumCh);

int main(int argc, char* argv[])
{
    char *ver = "0.5.1";
    int NumCh = 0;
    char *titkos_uzenet = "";
    char *array = "";

    if(argc == 1)
    {
        int f = BrowseForOpen();

        array = ReadPixels(f, &NumCh);

        titkos_uzenet = Unwrap(array, NumCh);

        Translate(titkos_uzenet, NumCh);

        return 0;
    }
    else if(strcmp(argv[1],"--version") == 0)
    {
        printf("version: %s\nDate: 2021.04.14\nMade by: Varadi Sandor\n",ver);
    }
    else if(strcmp(argv[1],"--help") == 0)
    {
        printf("--version\t\tVersion, Release Date, Author\n");
        printf("--help\t\t\tPossible Arguments\n");
        printf("Other arguments\t\tFile path\n");
        printf("No arguments\t\tFile browser\n");

        return 0;
    }
    else
    {
        FILE *file;
        file = fopen(argv[1], "rb");

        if (!file)
        {
            puts("\nWrong FILE path!\n");
            exit(1);
        }

        int fn = fileno(file);

        array = ReadPixels(fn, &NumCh);

        titkos_uzenet = Unwrap(array, NumCh);

        Translate(titkos_uzenet, NumCh);

        fclose(file);

        return 0;
    }
}