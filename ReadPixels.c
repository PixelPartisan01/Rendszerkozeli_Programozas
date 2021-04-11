#include "ReadPixels.h"
#include <stdio.h>

char* ReadPixels(int f, int* NumCh)
{
    char *buffer;
    long length;

    FILE* file = fdopen(f,"rb");

    if(!file)
    {
        printf("Failed to open file!\n");
        exit(1);
    }
    else
    {
        fseek (file, 0, SEEK_END);
        length = ftell (file);

        fseek (file, 6, SEEK_SET);
        fread(NumCh, 4, 1, file);

        buffer = malloc((length-54)*8);

        if(buffer == NULL)
        {
            printf("Memory is not allocated!\n");
            exit(1);
        }

        fseek (file, 54, SEEK_SET);
        fread(buffer, length-54, 1, file);

        char *ff = malloc(1);
        char fff;
        int buffer_index = 0;

        for(int j = 0; j < length-54; j++)
        {
            fseek (file, 54+j, SEEK_SET);
            fread(ff, 1, 1, file);

            char fff = *ff;

            for (int i = 7; i >= 0; --i)
            {
                if(fff & (1 << i))
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

        fclose (file);

        return buffer;
    }
}
