#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

int puts(const char *s);
int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("Usage: %s [hostname] [port]\n", argv[0]);
		exit(1);
	}

	int port = atoi(argv[2]);
	struct hostent *ip;
	ip = gethostbyname(argv[1]);

	if(&ip == NULL)
	{
		printf("hostname error\n");
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

	char addr[14];
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(ip->h_addr);

	int con = connect(client_fd, (struct sockaddr *) &server, sizeof(server));
	if(con < 0)
	{
		printf("Unable to connect\n");
		exit(1);
	}
	else
		printf("Connected\n");

	char buffer[100];//length of buffer//

	
	
	while(1)
	{
		printf("Enter command to write to server. \"quit\" to exit\n");
		fgets(buffer, 100, stdin);
		if(strcmp(buffer, "quit") == 0)
			break;//user finished writing//

		int err;
		err = write(client_fd, buffer, 100);
	
		if(err < 0)
		{
			printf("Failed to write to server\n");
			exit(1);
		}

		FILE *file;
		int rd = read(client_fd, file, 100);//attempt to read the first 100 bytes of the file//
		if(rd == 0)
		{
			printf("server not responding\n");
			exit(1);
		}
		while(rd > 0)//loop until EOF is reached//
		{
			rd = read(client_fd, file, 100);
			if(rd < 0)
			{
				printf("Cannot read from server\n");
				exit(1);
			}
		}

		
	}
	close(client_fd);
	printf("Connection closed\n");
}
