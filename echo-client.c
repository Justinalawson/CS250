#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

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

	char port[4], ip[15];
	printf("Please enter port\n");
	fgets(port, 15, stdin);
	
	printf("please enter IP\n");
	fgets(ip, 15, stdin);

	struct sockaddr server;
	server.sa_family = AF_INET;
	//server.sa_data = htonl(addr);

	int con = connect(client_fd, (struct sockaddr *) &server, sizeof(server));
	if(con < 0)
	{
		printf("Connection failed\n");
		exit(1);
	}
	else
		printf("Connected\n");

	char buffer[1024];//length of echo-server buffer//

	
	
	while(1)
	{
		printf("Enter message to write to server. \"quit\" to exit\n");
		fgets(buffer, 1024, stdin);
		if(strcmp(buffer, "string") == 0)
			break;//user finished writing//

		int err;
		err = write(client_fd, buffer, 1024);
	
		if(err < 0)
		{
			printf("Failed to write to server\n");
			exit(1);
		}

		read(client_fd, buffer, 1024);
		printf("Reply: ");
		puts(buffer);
	}

	close(client_fd);
	printf("Connection closed\n");
}
