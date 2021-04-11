#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ReadPixels.h"
#include "Unwrap.h"
#include "BrowseForOpen.h"
//#include "Post.h"

int main(int argc, char* argv[])
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    char *ver = "0.4.1";

    int NumCh = 0;
    char *titkos_uzenet = "";
    char *array = "";

    if(argc == 1)
    {
        int f = BrowseForOpen();

        array = ReadPixels(f, &NumCh);

        titkos_uzenet = Unwrap(array, NumCh);

        Translate(titkos_uzenet, NumCh);

        getchar();

        return 0;
    }
    else if(strcmp(argv[1],"--version") == 0)
    {
        printf("version: %s\nDate: 2021.04.05\nMade by: Varadi Sandor\n",ver);
        getchar();
    }
    else if(strcmp(argv[1],"--help") == 0)
    {
        printf("--version\t\tVersion, Release Date, Author\n");
        printf("--help\t\t\tPossible Arguments\n");
        printf("Other arguments\t\tFile name\n");
        printf("No arguments\t\tFile browser\n");
        getchar();
    }
    else
    {
        FILE *file;
        file = fopen(argv[1], "rb");

        int fn = fileno(file);

        array = ReadPixels(fn, &NumCh);

        titkos_uzenet = Unwrap(array, NumCh);

        Translate(titkos_uzenet, NumCh);

        fclose(file);

        getchar();
    }
    return 0;
}

void Translate (char *titkos_uzenet, int NumCh)
{
    int z = 0;

    char *NEPTUN = "HJTBVA";

    char resz_string[9];

    char uzenet[NumCh+1];

    uzenet[NumCh] = '\0';

    for(int i = 0; i < NumCh; i++)
    {
        memcpy(resz_string, &titkos_uzenet[z], 8); // Rész string
        resz_string[8] = '\0';
        char c = strtol(resz_string, 0, 2); // rész string (bináris kód) átalakítása karakterré
        uzenet[i] = c;
        z += 8;
    }

    //printf("%s", uzenet);

    Post(NEPTUN, uzenet, NumCh);
}
