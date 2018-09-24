#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/types.h>
//#include <linux/if_packet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include "header.h"

void ProcessPacket(unsigned char*, int);
void print_ip_header(unsigned char*, int);
void print_udp_packet(unsigned char*, int);
void PrintData(unsigned char*, int);

static int sock_fd;
FILE *logfile;
int udp=0, rssi = 0, total = 0, i,j;
static struct sockaddr_in source;
static struct sockaddr_in dest;

int main(void)
{
	int src_addr_size, data_size;
	struct sockaddr src_addr;
	struct in_addr in;

	

	unsigned char *buffer = (unsigned char *)malloc(65536);

	logfile = fopen("log.txt","w");
	if (logfile == NULL)
		printf("Unable to open file.\n");
	else
		printf("Starting...\n");
	
	sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sock_fd < 0)
	{	
		printf("Socket Error\n");
		printf("socket output: %d\n", sock_fd);
		exit(1);
	}
	while(1)
	{
		src_addr_size = sizeof(src_addr);
		printf("Waiting to recvfrom...\n");
		data_size = recvfrom(sock_fd, buffer, 65535, 0, &src_addr, &src_addr_size);
		printf("Received!...\n");
		if(data_size < 0)
		{
			printf("Recvfrom error...\n");
			exit(1);
		}
		
		ProcessPacket(buffer, data_size);
	}

	close(sock_fd);
	fclose(logfile);
	printf("Finished");
	return 0;
}

void ProcessPacket(unsigned char* buffer, int size)
{
	struct ipheader *ip = (struct ipheader *)buffer;
	++total;
	switch (ip->iph_protocol)
	{
		case 17:
			++udp;
			print_udp_packet(buffer, size);
			break;
		
		default: //rssi
			++rssi;
			break;
	}

}

void print_ip_header(unsigned char* buffer, int size)
{
	unsigned short int header_len;

	struct ipheader *ip = (struct ipheader *)buffer;
	header_len = ip->iph_len;

	memset(&source, 0, sizeof(source));
	memset(&dest, 0, sizeof(dest));

	source.sin_addr.s_addr = ip->iph_sourceip;
	dest.sin_addr.s_addr = ip->iph_destip;

	fprintf(logfile, "\n");
	fprintf(logfile, "IP Header \n");
	fprintf(logfile,"   |-IP Version        : %c\n",(int)ip->iph_ver);
    fprintf(logfile,"   |-IP Header Length  : %c Bytes\n",((int)(ip->iph_ihl))*4);
    fprintf(logfile,"   |-Type Of Service   : %d\n",(int)ip->iph_tos);
    fprintf(logfile,"   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(ip->iph_len));
    fprintf(logfile,"   |-Identification    : %d\n",ntohs(ip->iph_ident));
	fprintf(logfile,"   |-TTL      : %d\n",(unsigned int)ip->iph_ttl);
    fprintf(logfile,"   |-Protocol : %d\n",(unsigned int)ip->iph_protocol);
    fprintf(logfile,"   |-Checksum : %d\n",ntohs(ip->iph_chksum));
    fprintf(logfile,"   |-Source IP        : %s\n",inet_ntoa(source.sin_addr));
    fprintf(logfile,"   |-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));
}
	

void print_udp_packet(unsigned char* buffer, int size)
{
	unsigned short iphdrlen;
     
    struct ipheader *ip = (struct ipheader *)buffer;
    iphdrlen = ip->iph_len;
     
    struct udpheader *udp = (struct udpheader*)(buffer + iphdrlen);
     
    fprintf(logfile,"\n\n***********************UDP Packet*************************\n");
     
    print_ip_header(buffer,size);           
     
    fprintf(logfile,"\nUDP Header\n");
    fprintf(logfile,"   |-Source Port      : %d\n" , ntohs(udp->udph_srcport));
    fprintf(logfile,"   |-Destination Port : %d\n" , ntohs(udp->udph_destport));
    fprintf(logfile,"   |-UDP Length       : %d\n" , ntohs(udp->udph_len));
    //fprintf(logfile,"   |-UDP Checksum     : %d\n" , ntohs(udph->check));
     
    fprintf(logfile,"\n");
    fprintf(logfile,"IP Header\n");
    PrintData(buffer , iphdrlen);
         
    fprintf(logfile,"UDP Header\n");
    PrintData(buffer+iphdrlen , sizeof(udp));
         
    fprintf(logfile,"Data Payload\n");  
    PrintData(buffer + iphdrlen + sizeof(udp) ,( size - sizeof(udp) - ip->iph_len ));
     
    fprintf(logfile,"\n###########################################################");
}

void PrintData (unsigned char* data , int size)
{
     
    for(i=0 ; i < size ; i++)
    {
        if( i!=0 && i%16==0)   //if one line of hex printing is complete...
        {
            fprintf(logfile,"         ");
            for(j=i-16 ; j<i ; j++)
            {
                if(data[j]>=32 && data[j]<=128)
                    fprintf(logfile,"%c",(unsigned char)data[j]); //if its a number or alphabet
                 
                else fprintf(logfile,"."); //otherwise print a dot
            }
            fprintf(logfile,"\n");
        } 
         
        if(i%16==0) fprintf(logfile,"   ");
            fprintf(logfile," %02X",(unsigned int)data[i]);
                 
        if( i==size-1)  //print the last spaces
        {
            for(j=0;j<15-i%16;j++) fprintf(logfile,"   "); //extra spaces
             
            fprintf(logfile,"         ");
             
            for(j=i-i%16 ; j<=i ; j++)
            {
                if(data[j]>=32 && data[j]<=128) fprintf(logfile,"%c",(unsigned char)data[j]);
                else fprintf(logfile,".");
            }
            fprintf(logfile,"\n");
        }
    }
}
