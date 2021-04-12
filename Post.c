#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib") //Winsock Library
#define BUFSIZE 1024

int Post(char *NeptunId , char *message, int NumCh)
{

	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char buffer[BUFSIZE];
	char server_reply[2000];
	int recv_size;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
	    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		printf("Failed. Error Code : %d",WSAGetLastError());
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		getchar();
		exit(1);
	}

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	printf("Initialised.\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	//Create a socket
	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
	    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		printf("Could not create socket : %d" , WSAGetLastError());
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		getchar();
		exit(1);
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	printf("Socket created.\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);


	server.sin_addr.s_addr = inet_addr("193.6.135.162");
	server.sin_family = AF_INET;
	server.sin_port = htons(80);

	//Connect to remote server
	if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
	    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		puts("Connect Error");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		exit(1);
	}

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	puts("Connected");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	//Send some data
    sprintf(buffer, "POST /~vargai/post.php HTTP/1.1\r\nHost: irh.inf.unideb.hu\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nNeptunID=%s&PostedText=%s", (NumCh+27), NeptunId, message);

    printf("\n%s\n", buffer);
	if( send(s , buffer , strlen(buffer) , 0) < 0)
	{
	    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		puts("Send failed");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		exit(1);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	puts("Data Send\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	//Receive a reply from the server
	if((recv_size = recv(s , server_reply , 2000 , 0)) == SOCKET_ERROR)
	{
	    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		puts("Recv Failed");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		exit(1);
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	puts("Reply received\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	//Add a NULL terminating character to make it a proper string before printing
	server_reply[recv_size] = '\0';

	if(strstr(server_reply, "The message has been received.") == NULL)
    {
        fflush(stdout);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
        puts("Server Reply Error\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        puts("Server Reply:\n");

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4+15*16 | FOREGROUND_INTENSITY);
        puts(server_reply);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        exit(1);
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4+15*16 | FOREGROUND_INTENSITY);
    puts(server_reply);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	closesocket(s);
    WSACleanup();

    getchar();
	return 0;
}
