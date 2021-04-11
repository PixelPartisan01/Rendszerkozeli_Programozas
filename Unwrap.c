#include <stdio.h>
#include "Unwrap.h"

char* Unwrap(char* Pbuff, int NumCh)
{
    char *kod = (char*)malloc(NumCh*8);
    if(kod == NULL)
    {
        printf("Memory is not allocated!\n");
        exit(1);
    }

    int kodIndex = 0;
    int Pbuff_index = 0;
    char reszlet[8];
    int rgb = 1;

    for(int i = 0; i < 24*NumCh; i+=8)
    {
        for(int i = 0; i < 8; i++)
        {
            reszlet[i] = Pbuff[Pbuff_index+i];
        }

        Pbuff_index += 8;

        if(rgb == 1)
        {
            kod[kodIndex] = reszlet[6];
            kodIndex++;
            kod[kodIndex] = reszlet[7];
            kodIndex++;

            rgb=2;
        }
        else if(rgb == 2)
        {
            kod[kodIndex] = reszlet[5];
            kodIndex++;
            kod[kodIndex] = reszlet[6];
            kodIndex++;
            kod[kodIndex] = reszlet[7];
            kodIndex++;

            rgb=3;
        }
        else if(rgb == 3)
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

