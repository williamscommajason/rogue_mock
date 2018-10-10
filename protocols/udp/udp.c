#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <linux/if_ether.h>

#include "header.h"

unsigned short checksum(unsigned short *buf, int nwords)
{
    unsigned long sum;
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);

}

unsigned int build_ip(struct in_addr source_addr, struct in_addr dest_addr, uint8_t protocol, uint8_t *ip_packet, 
uint8_t *data, unsigned int data_size)

{

	struct ipheader *ip;

	ip = (struct ipheader *)ip_packet;
	ip->iph_ver = 4;
	ip->iph_tos = 0;
	ip->iph_len = htons(sizeof(struct ipheader) >> 2 + data_size);
	ip->iph_ident = 0; //filled in by kernel
	ip->iph_ttl = 64;
	ip->iph_protocol = protocol;
	ip->iph_chksum = 0; // done by kernel
	ip->iph_sourceip = source_addr.s_addr;
	ip->iph_destip = dest_addr.s_addr;
	ip->iph_ihl = INET_HDR_LEN;

	memcpy(ip_packet + sizeof(struct ipheader), data, data_size);

	return sizeof(struct ipheader) + data_size;

}

unsigned int build_udp(struct sockaddr_in source_addr, struct sockaddr_in dest_addr, uint8_t *udp_packet, uint8_t *data, unsigned int data_size)

{
	struct udpheader *udp;
	int len;
	len = UDP_HDR_SIZE + data_size;

	udp = (struct udpheader *)udp_packet;
	udp->udph_srcport = source_addr.sin_port;
	udp->udph_destport = dest_addr.sin_port;
	udp->udph_len = htons(len);
	memcpy(udp_packet + UDP_HDR_SIZE, data, data_size);

	return len;

}

void send_udp(int sock_fd, struct sockaddr_in source_addr, struct sockaddr_in dest_addr, uint8_t *data, unsigned int data_size)

{
	int flag = 1;
	unsigned int packet_size;
	unsigned int ip_payload_size;
	uint8_t packet[ETH_DATA_LEN];
	int ip_plus_payload;

	memset(packet, 0, ETH_DATA_LEN);
	ip_plus_payload = build_udp(source_addr, dest_addr, packet + sizeof(struct ipheader), data, data_size);

	packet_size = build_ip(source_addr.sin_addr, dest_addr.sin_addr, IPPROTO_UDP, packet, packet + sizeof(struct ipheader), ip_plus_payload);

	if(sendto(sock_fd, packet, packet_size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
	{
		perror("Sendto failed!");
		exit(1);
	}
}
