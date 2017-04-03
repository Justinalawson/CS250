#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

//int gets(char *s);
int puts(const char *s);
int main()
{
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd < 0)
	{
		printf("Socket could not be created\n");
		exit(1);
	}
	else
		printf("Socket created\n");

	char port[4], ip[14];
	printf("Please enter port\n");
	fgets(port, 5, stdin);

	fflush(stdin);
	
	printf("please enter IP\n");
	fgets(ip, 15, stdin);

	struct sockaddr_in server;
	struct in_addr addr;
	inet_aton(ip, &addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr = addr;

	char buffer[1024];//length of echo-server buffer//

	while(1)
	{
		printf("Enter message to write to server. \"quit\" to exit\n");
		fgets(buffer, 1024, stdin);
		if(strcmp(buffer, "quit") == 0)
		{
			printf("Done Writing\n");
			break;//user finished writing//
		}

		int err;
		
		err = sendto(client_fd, buffer, 1024, 0, (struct sockaddr *) &server, sizeof(server));
		
		if(err < 0)
		{
			printf("Failed to write to server\n");
			exit(1);
		}
			
		printf("Message Sent\n");

		err = recvfrom(client_fd, buffer, 1024, 0, (struct sockaddr *) &server, sizeof(server));
		
		if(err < 0)
		{
			printf("Server not responding\n");
			exit(1);
		}

		printf("Reply: ");
		puts(buffer);
	}

	close(client_fd);
	printf("Connection closed\n");
}
