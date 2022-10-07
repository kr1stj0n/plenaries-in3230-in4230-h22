#ifndef ETHERNET_H_
#define ETHERNET_H_

#include <stdint.h>

#define ETH_DST_MAC {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}
#define ETH_P_HIP 0xFFFF
#define ETH_HDR_LEN sizeof(struct eth_hdr)

struct eth_hdr {
	uint8_t  dst_mac[6];
	uint8_t  src_mac[6];
	uint16_t ethertype;
	uint8_t  payload[];
} __attribute__((packed));

#endif /* ETHERNET_H_*/
