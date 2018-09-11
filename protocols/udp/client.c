#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MY_PORT "4950"


int client(int argc, char *argv[])
{
	int sock_fd, numbytes, ad;
	struct addrinfo hints, *servinfo, *p;
		
	if (argc != 3)

	{
		fprintf(stderr,"Usage is: %s <domain_name> <message> \n", argv[0]);
		exit(1);
	}

	/*
	if ((h = gethostbyname(argv[1])) == NULL)
	{
		herror("gethostbyname(): ");
	}
	else
	{
		printf("Retrieved host name...");
	}


	printf("Hostname: %s\n", h->h_name);
	printf("IP Address: %s\n", inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));
	printf("The address length is: %d\n", h->h_length);
	
	int j = 0;

	do
	{
		printf("Alias #%d: %s \n",j,h->h_aliases[j]);
		j++;
	
	}	while(h->h_aliases[j] != NULL);

	printf("Checking for other IPs.... \n");

	int i = 1;
	do
	{
		printf("Address #%i is: %s\n", i, inet_ntoa(*((struct in_addr *)(h->h_addr_list[i]))));
		i++;
	
	}	while((h->h_addr_list[i]) != NULL);
	
	*/
	
	if ((ad = getaddrinfo(argv[1], MY_PORT, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ad));
		return 1;
	}

	// loop through all results and make a socket

	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("Client: socket");
			continue;
		}
		
		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "Client: Failed to create socket \n");
		return 2;
	}

	numbytes = sendto(sock_fd, argv[2], strlen(argv[2]), 0, p->ai_addr, p->ai_addrlen);
	
	if (numbytes == -1)
	{
		perror("Client- sendto() failed");
		exit(1);
	}
	else

	freeaddrinfo(servinfo);

	printf("Sent %d bytes to %s \n", numbytes, argv[1]);
	close(sock_fd);

	return 0;
}

int main(int argc, char *argv[])
{
	client(argc, argv);
	return 0;
}
