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
#include "header.h"

#define MY_PORT "4950"
#define PCKT_LEN 8192


unsigned short checksum(unsigned short *buf, int nwords)
{
	unsigned long sum;
	for (sum = 0; nwords > 0; nwords--)
		sum += *buf++;
	sum = (sum >> 16) + (sum &0xffff);
	sum += (sum >> 16);
	return (unsigned short)(~sum);

}

int client(int argc, char *argv[])
{
	int sock_fd, numbytes, ad;
	char buffer[PCKT_LEN];
	struct addrinfo hints, *servinfo, *p;
	struct ipheader *ip = (struct ipheader *)buffer;
	struct udpheader *udp = (struct udpheader *) (buffer + sizeof(struct ipheader));
	
	memset(buffer, 0, PCKT_LEN);
	hints.ai_family = AF_INET;	
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_UDP;

	if (argc != 5)

	{
		fprintf(stderr,"Usage is: %s <source hostname/IP> <source port> <target hostname/IP> <target port>  \n", argv[0]);
		exit(1);
	}

	/*	remember that ports are not a concept in IP, so port number
		for getaddrinfo is NULL, we instead place it in header
	*/
		
	if ((ad = getaddrinfo(argv[1], NULL, &hints, &servinfo)) != 0)
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
	
	//create ip header
	ip->iph_ihl = 5;
	ip->iph_ver = 4;
	ip->iph_tos = 16;
	ip->iph_len = sizeof(struct ipheader) + sizeof(struct udpheader);
	ip->iph_ident - htons(54321);
	ip->iph_protocol = 17;
	ip->iph_sourceip = inet_addr(argv[1]);
	ip->iph_destip = inet_addr(argv[3]);
	
	//create udp header
	udp->udph_srcport = htons(atoi(argv[2]));
	udp->udph_destport = htons(atoi(argv[4]));
	udp->udph_len = htons(sizeof(struct udpheader));
	
	//Calculate checksum
	ip->iph_chksum = checksum((unsigned short *)buffer, sizeof(struct ipheader) + sizeof(struct udpheader));

	//Inform kernal to not fill packet with their own header...
	int one = 1;
	const int *val = &one;
	if (setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
	{
		perror("setsockopt() error");
		exit(-1);	
	}
	else
	printf("setsockopt() succeeded!");



	numbytes = sendto(sock_fd, buffer, ip->iph_len, 0, p->ai_addr, p->ai_addrlen);
	
	if (numbytes == -1)
	{
		perror("Client- sendto() failed");
		exit(1);
	}
	

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
