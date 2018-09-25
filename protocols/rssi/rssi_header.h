unsigned int build_rssi

struct rssiheader 
{

	unsigned char	rssih_syn:1;
	unsigned char	rssih_ack:1;
	unsigned char	rssih_eac:1;
	unsigned char 	rssih_rst:1;
	unsigned char	:0;
	unsigned char	:0;
	unsigned char	rssih_busy:1;

	unsigned short	rssih_len:8; //always set to 8
	
	unsigned char	rssih_seqnum;
	unsigned char	rssih_acknum;

	unsigned short	rssih_spare;

	unsigned short	rssih_chksum;
}

struct SYN
{

	unsigned char	syn_first:1; // always set to 1
	unsigned char	syn_ack:1;
	unsigned char	:0;
	unsigned char   :0;
	unsigned char   :0;
	unsigned char   :0;
	unsigned char   :0;
	unsigned char   :0;
	unsigned short	syn_len:8; //always set to 24
	unsigned short 	syn_seqnum:8;
	unsigned short	syn_acknum:8;
	unsigned short	syn_ver:4;
	unsigned char	syn_second:1; // always set to 1
	unsigned char	syn_chk:1
	unsigned char	:0;
	unsigned char	:0;
	unsigned short	syn_maxoutseg:8;
	unsigned short	syn_maxseg;

	unsigned short	syn_rtt;
	unsigned short 	syn_cat;
	unsigned short	syn_nt;
	unsigned short	syn_maxrt:8;
	unsigned short	syn_maxca:8;

	unsigned short	syn_maxoosa:8;
	unsigned short	syn_tu:8;
	unsigned short	syn_idmsb;
	unsigned short	syn_idlsb;
	unsigned short	syn_chksum;

}
		

		
	
