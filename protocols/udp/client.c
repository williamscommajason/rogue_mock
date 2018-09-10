#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MY_PORT 4950

int client(int argc, char **argv[])
{
	int sock_fd, numbytes;
	struct hostent *h;
	struct sockaddr_in client_addr;
	
	if (argc != 3)

	{
		fprintf(stderr,"Usage is: %s <domain_name> <message> \n", (const char *)argv[0]);
		exit(1);
	}

	/*now get host into */
	if ((h = gethostbyname((const char *)argv[1])) == NULL)
	{
		herror("gethostbyname(): ");
	}
	else
	{
		printf("Retrieved host name...");
	}

	/* now we can print out hostnames */

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
	
	return 0;

	if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("Client - socket failed to open");
		exit(1);
	}
	else
	{
		printf("Socket open\n");
	}
	
	client_addr.sin_port = AF_INET;
	client_addr.sin_port = htons(MY_PORT);
	client_addr.sin_addr = *((struct in_addr *)h->h_addr_list[0]);
	memset(&(client_addr.sin_zero),0,8);

	numbytes = sendto(sock_fd, (const char*)argv[2], strlen((const char *)argv[2]), 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));
	
	if (numbytes == -1)
	{
		perror("Client- sendto() failed");
		exit(1);
	}
	else
	{
		printf("Message sent!\n");
	}

	printf("Sent %d bytes to %s \n", numbytes, inet_ntoa(client_addr.sin_addr));

	return 0;
}

