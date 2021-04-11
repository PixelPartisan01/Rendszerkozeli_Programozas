#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "BrowseForOpen.h"

int BrowseForOpen()
{
    FILE *file;
    DIR *d;
    char keres[] = "";
    struct dirent *entry;
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    while(1)
    {
        d = opendir(cwd);

        if(!d)
        {
            if(strstr(cwd,".bmp") != NULL && !(strlen(keres) > 4))
            {
                file = fopen(cwd, "rb");
                int fn = fileno(file);
                return fn;
            }
            else
            {
                strcat(cwd, "\\..");
            }
        }
        else
        {
            while((entry=readdir(d))!=NULL)
            {
                if((*entry).d_name[0]!='.')
                    printf("%s\n",(*entry).d_name);
            }

            closedir(d);

            printf("\n\nSearch: ");
            scanf("%s", keres);

            if(strchr(keres, ':') != NULL)
            {
                system("cls");
                file = fopen(keres, "rb");
                int fn = fileno(file);
                return fn;
            }

            strcat(cwd, "\\");
            strcat(cwd, keres);

            system("cls");
        }
   }
}
