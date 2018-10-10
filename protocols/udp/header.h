#ifndef UDP_H
#define UDP_H

#include <stdint.h>
#include <arpa/inet.h>

#define UDP_HDR_SIZE 8
#define INET_HDR_LEN 5

unsigned int build_udp(struct sockaddr_in src_addr, struct sockaddr_in dst_addr, uint8_t *udp_packet, uint8_t *data, unsigned int data_size);

unsigned int build_ip(struct in_addr src_addr, struct in_addr dst_addr, uint8_t protocol, uint8_t *ip_packet, uint8_t *data, unsigned int data_size);

void send_udp(int raw_sock, struct sockaddr_in src_addr, struct sockaddr_in dst_addr, uint8_t *data, unsigned int data_size);


struct ipheader
{
	unsigned char		iph_ihl:5, iph_ver:4;
	unsigned char		iph_tos;
	unsigned short int	iph_len;
	unsigned short int	iph_ident;
	unsigned char		iph_flag;
	unsigned short int 	iph_offset;
	unsigned char		iph_ttl;
	unsigned char		iph_protocol;
	unsigned short int	iph_chksum;
	uint32_t		 	iph_sourceip;
	uint32_t			iph_destip;
}; /* total ipheader length: 21 bytes ( = 168 bits) */



struct udpheader
{
	unsigned short int udph_srcport;
	unsigned short int udph_destport;
	unsigned short int udph_len;
	//unsigned short int udph_chksum;
}; /* total udp header length: 8 bytes (= 64 bits) */

#endif
