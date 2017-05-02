#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
int puts(const char *s);
int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s [hostname/IP] [port]\n", argv[0]);
		exit(1);
	}

	int server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP), client_fd;
	if(server_fd < 0)
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

	struct sockaddr_in server, client;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	socklen_t len = sizeof(client);

	int bnd = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
	if(bind < 0)
	{
		printf("Could not bind socket\n");
		exit(1);
	}

	else
		printf("Socket bound on %d\n", port);

	char buffer[1024];
	
	int broadcast = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_BROADCAST, (void *) &broadcast, sizeof(broadcast));
	while(1)
	{
		int status = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &client, &len);
		
		if(status < 0)
		{
			printf("Read Failed\n");
			exit(1);
		}

		status = sendto(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &client, &len);
		
		if(status < 0)
		{
			printf("Send Failed\n");
			exit(1);
		}
	}

	close(server_fd);
}
