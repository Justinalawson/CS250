#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
int puts(const char *s);
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s [port]\n", argv[0]);
		exit(1);
	}

	int port = atoi(argv[1]);

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
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	int bnd = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
	if(bind < 0)
	{
		printf("Could not bind socket\n");
		exit(1);
	}

	else
		printf("Socket bound on port %s\n", argv[1]);

	int lst = listen(server_fd, 128);
	if(listen < 0)
	{
		printf("Could not listen on socket\n");
		exit(1);
	}

	else
		printf("Listening on socket\n");

	char buffer[100];//length of buffer//
	
	while(1)//accept connection//
	{
		socklen_t clientSize = sizeof(client);

		int acc = accept(server_fd, (struct sockaddr *) &client, &clientSize);
		if(acc < 0)
		{
			printf("Failed to connect\n");
			exit(1);
		}

		else
			printf("Connected\n");

		pid_t pid;

		while(1)//read and execute command//
		{
			int rd = read(client_fd, buffer, sizeof(buffer));
			if(!read)
				break;

			if(read < 0)
			{
				printf("Failed to read from client\n");
				exit(1);
			}

			FILE *file = fopen(buffer, "r");//attempt to open requested file//
			if(file == NULL)
			{
				printf("File not found\n");
				break;
			}

			int fd = fileno(file);
			int snd = sendfile(fd, client_fd, 0, sizeof(file));

			if(snd < 0)
			{
				printf("Failed to send file\n");
				exit(1);
			}
		}
	}
}
