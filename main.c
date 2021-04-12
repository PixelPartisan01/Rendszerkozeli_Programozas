#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <linux/limits.h>
#include <arpa/inet.h>
#define BUFSIZE 1024 
#define PORT_NO 80

int BrowseForOpen();
char* ReadPixels(int f, int* NumCh);
char* Unwrap(char* Pbuff, int NumCh);
void Translate (char *titkos_uzenet, int NumCh);
int Post(char *NeptunId , char *message, int NumCh);

int main(int argc, char* argv[])
{
    char *ver = "0.4.2";

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
        printf("version: %s\nDate: 2021.04.12\nMade by: Varadi Sandor\n",ver);
    }
    else if(strcmp(argv[1],"--help") == 0)
    {
        printf("--version\t\tVersion, Release Date, Author\n");
        printf("--help\t\t\tPossible Arguments\n");
        printf("Other arguments\t\tFile name\n");
        printf("No arguments\t\tFile browser\n");
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

int BrowseForOpen()
{
    FILE *file;
    DIR *d;
    char keres[1000] = "";
    char konyvtar[1000] = ".";
    struct dirent *entry;


    while(1)
    {
        d = opendir(konyvtar);

        printf("%s\n\n", konyvtar);
        
        if(!d)
        {
            if(strstr(keres,".bmp"))
            {
                file = fopen(konyvtar, "rb");
                int fn = fileno(file);
                return fn;
            }
            else
            {
                strcat(konyvtar, "/..");
            }
        }
        else
        {
            while((entry=readdir(d))!=NULL)
            {
                printf("%s\n",(*entry).d_name);
            }

            closedir(d);
            
            printf("\n\nSearch: ");
            scanf("%s",keres);

            strcat(konyvtar, "/");
            strcat(konyvtar, keres);
        }
    }
}

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

int Post(char *NeptunId , char *message, int NumCh)
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

    on   = 1;
    flag = 0;
    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = inet_addr("193.6.135.162");
    server.sin_port        = htons(PORT_NO);
    server_size = sizeof server;

    s = socket(AF_INET, SOCK_STREAM, 0 );
    
    if ( s < 0 ) 
    {
        fprintf(stderr, " %s: Socket creation error.\n");
        exit(2);
    }

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    err = connect(s, (struct sockaddr *) &server, server_size);

    if ( err < 0 ) 
    {
        fprintf(stderr, " %s: Connecting error.\n");
        exit(3);
    }

    printf(" Connection is OK.\n");

    printf(" Message to send: ");
    
    sprintf(buffer, "POST /~vargai/post.php HTTP/1.1\nHost: irh.inf.unideb.hu\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nNeptunID=%s&PostedText=%s", (NumCh+27), NeptunId, message);

    printf("\n\n%s\n", buffer);

    bytes = send(s, buffer, strlen(buffer)+1, flag);

    if ( bytes <= 0 ) 
    {
        fprintf(stderr, " %s: Sending error.\n");
        exit(4);
    }
    
    if( recv(s , server_reply , 2000 , 0) < 0)
	{
		puts("recv failed");	
    	exit(1);
	}
    else if (strstr(server_reply, "The message has been received.") == NULL)
    {
        printf("\n");
        puts(server_reply);
        exit(1);
    }

    printf (" %i bytes have been sent to server.\n", bytes-1);

    bytes = recv(s, buffer, BUFSIZE, flag);

    if ( bytes < 0 ) 
    {
        fprintf(stderr, " %s: Receiving error.\n");
        exit(5);
    }

    printf(" Server's (%s:%d) acknowledgement:\n  %s\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port), buffer);


    close(s);

    return EXIT_SUCCESS;
}