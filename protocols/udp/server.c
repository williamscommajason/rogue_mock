#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>


#define MYPORT 3334
#define BACKLOG 10
#define MAXBUFF 500

int sock(int argc, char *argv[])
{
	int sock_fd, addr_len, numbytes;
	struct sockaddr_in my_addr, their_addr;
	char buf[MAXBUFF];

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
	{
		perror("Error :");
		exit(1);
	}
	else
		printf("Socket is open \n");

	// my_addr.sin_len = 
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(my_addr.sin_zero), 0, 8);
	
	addr_len = sizeof(struct sockaddr);
	
	if ((numbytes = recvfrom(sock_fd, buf, MAXBUFF - 1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1)
	{
		perror("Server-recvfrom():");
	}
	else
	{
		printf("Packet received using recvfrom() \n");
	}
	printf("Server: Got packed from %s \n", inet_ntoa(their_addr.sin_addr));
	printf("Server: Packet is %d bytes long \n" ,numbytes);
	buf[numbytes] = '\0';
	
	
	
	if (bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{
		perror("Error :");
		exit(1);
	}
	else
	{
		printf("Socket is binded to port \n");
	}
	//printf("stopping now?");	
	/*if (listen(sock_fd, BACKLOG) == -1)
	{
		perror("Error :");
		exit(1);
	}
	else
	{	
		printf("Listening...");
	}

	new_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &sin_size);
	
	if (new_fd == -1)
	{
		perror("Accepting Client failed :(");
	}
	else 
	{
		printf("Client accpeted!");
	}
	*/
	close(sock_fd);	
	return 0;
}


int main(int argc, char *argv[])
{
	sock(argc, argv);
	return 0;
}

