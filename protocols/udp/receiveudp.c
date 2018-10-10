#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define MSG_SIZE 8192 
#define MYPORT 4950
#define MYADDR "127.0.0.1"

int main(void)
{
	static int sock_fd;
	struct sockaddr_in src_addr;
	socklen_t socklen;

	char msg[MSG_SIZE];
	
	ssize_t msglen;

	if ((sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1)
	{
		perror("socket");
		exit(1);
	}

	src_addr.sin_family = AF_INET;
	src_addr.sin_port = htons(MYPORT);
	src_addr.sin_addr.s_addr = inet_addr(MYADDR);
	
	socklen = (socklen_t) sizeof(src_addr);

	if (bind(sock_fd, (struct sockaddr *)&src_addr, socklen) == -1)
	{
		perror("bind");
		goto _close_socket;
	}

	memset(msg, 0, MSG_SIZE);
	
	if ((msglen = recv(sock_fd, msg, MSG_SIZE, 0)) == -1)
	{
		perror("recv");
		goto _close_socket;
	}

_close_socket:
	close(sock_fd);

	return 0;
}
