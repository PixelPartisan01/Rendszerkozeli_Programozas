#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SecretMessage.h"

int main(int argc, char* argv[])
{
    char *ver = "1.0.0"; //0.5.3
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
        printf("version: %s\nDate: 2021.04.19\nMade by: Varadi Sandor\n",ver);
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
            puts("\nFILE doesn't exist or wrong FILE path!\n");
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