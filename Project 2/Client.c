#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s [hostname/IP] [port]\n", argv[0]);
		exit(1);
	}

	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd < 0)
	{
		printf("Socket could not be created\n");
		exit(1);
	}
	else
		printf("Socket created\n");

	int port = atoi(argv[2]);
	
	if(port == 0)
	{
		printf("Invalid Port\n");
		exit(1);
	}

	in_addr_t ip = inet_addr(argv[1]);

	if(ip == (in_addr_t) -1)
	{
		printf("Invalid hostname/IP\n");
		exit(1);
	}

	struct sockaddr_in server;
	struct in_addr addr;
	inet_aton(argv[1], &addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr = addr;

	char buffer_in[1024];
	char buffer_out[1024];
	
	printf("Welcome to the Chat Server. Enter QUIT to exit.\n");
	
	//Continuously read messages from server//
	if(fork() == 0)
	{
		while(1)
		{
			int recvErr = recvfrom(client_fd, buffer_in, 1024, 0, (struct sockaddr *) &server, sizeof(server));

			if(recvErr < 0)
			{
				//printf("Read error\n");
			}

			else
			{
				printf("%s", buffer_in);
			}
		}
	}
	while(1)
	{
		fgets(buffer_out, 1024, stdin);

		if(strcmp(buffer_out, "QUIT") == 0)
		{
			close(client_fd);
			exit(0);//User quits program//
		}
		

		time_t c_time = time(NULL);
		strcat(buffer_out, &c_time);
		int err;
		
		err = sendto(client_fd, buffer_out, sizeof(buffer_out), 0, (struct sockaddr *) &server, sizeof(server));
		
		if(err < 0)
		{
			printf("Write Error\n");
		}
	}
}
