#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT "4950"
#define BACKLOG 10
#define MAXBUFF 500

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}
	else
	{
		return &(((struct sockaddr_in6 *)sa)->sin6_addr);
	}
}


int sock(int argc, char *argv[])
{
	int sock_fd, ad, numbytes;
	/* 
	- hints points to an addrinfo struct that specifies criteria for selecting socket address returned in servinfo
	- servinfo is a pointer to an array (list) of socket address structs
	*/
	struct addrinfo hints, *servinfo, *p;
	char buf[MAXBUFF];
	struct sockaddr_storage their_addr;
	socklen_t addr_len;	
	char s[INET_ADDRSTRLEN];

	if (argc !=2) 
	{
		fprintf(stderr, "usage ./server hostname \n");
		exit(1);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; //use my IP

	if ((ad = getaddrinfo(NULL , MYPORT, &hints, &servinfo)) != 0)
	{
		fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(ad));
		return 1;
	}

	for(p = servinfo; p != NULL ; p = p->ai_next)
	{
		if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
		{
			perror("Server: socket");
			continue;
		}
		
		if (bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sock_fd);
			perror("Server: bind");
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "Server: Failed to bind socket \n");
		return 2;
	}
	
	freeaddrinfo(servinfo);
	
	printf("Server: waiting to recvfrom... \n");
	addr_len = sizeof(their_addr);	

	if ((numbytes = recvfrom(sock_fd, buf, MAXBUFF - 1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1)
	{
		perror("Server-recvfrom():");
	}
	else
	{
		printf("Packet received using recvfrom() \n");
	}
	//printf("Server: Got packed from %s \n", inet_ntoa(their_addr.sin_addr));
	printf("Server: Packet is %d bytes long \n" ,numbytes);
	buf[numbytes] = '\0';
	printf("Server: Packet contains \"%s\"\n", buf);
	
	
	if (close(sock_fd) != 0)
	{
		printf("Server cannot close! \n");
	}
	else
	{
		printf("Server successfully closed! \n");
	}	
	return 0;
}


int main(int argc, char *argv[])
{
	sock(argc, argv);
	return 0;
}

