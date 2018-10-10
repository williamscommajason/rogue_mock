#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include "header.h"

#define MAX_DATA_SIZE 1024

int main(void)

{
	int sock_fd;
	uint8_t packet[ETH_DATA_LEN];
	uint8_t udp_packet[ETH_DATA_LEN];
	uint8_t data[MAX_DATA_SIZE];
	char message[] = "Hello darkness my old friend.";
	char *lh = "127.0.0.1"; //local host
	char *sh = "127.0.0.1"; // source host
	unsigned int udp_size, ip_size;
	unsigned int data_size;
	struct sockaddr_in source_addr;
	struct sockaddr_in dest_addr;

	source_addr.sin_family = AF_INET;
	source_addr.sin_port = htons(5500);
	inet_aton(sh, &source_addr.sin_addr); //convert ascii string IP address and place in source_addr
	
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(4950);
	inet_aton(lh, &dest_addr.sin_addr);

	strcpy((char *)data, message);
	data_size = strlen(message);

	printf("Building UDP packet... \n");
	udp_size = build_udp(source_addr, dest_addr, udp_packet, data, data_size);
	
	printf("Building IP packet... \n");
	ip_size = build_ip(source_addr.sin_addr, dest_addr.sin_addr, IPPROTO_UDP, packet, udp_packet, udp_size);

	printf("Send UDP packet... \n");
	if ((sock_fd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) < 0)
	{
		perror("socket");
		exit(1);
	}

	send_udp(sock_fd, source_addr, dest_addr, data, data_size);

	return 0;

}
