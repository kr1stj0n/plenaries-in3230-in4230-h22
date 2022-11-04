#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>
#include <time.h>

#include <linux/if_packet.h>

#define BROADCAST_MAC_ADDR {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}

#define HELLO_INTERVAL 5000
#define HELLO_TIMEOUT 10000
#define MAX_EVENTS 5
#define MAX_CONNS 5

enum state {
    INIT = 0,
    WAIT,
    EXIT
};

struct ether_frame {
        uint8_t dst_addr[6];
        uint8_t src_addr[6];
        uint8_t eth_proto[2];
} __attribute__((packed));


double diff_time_ms(struct timespec, struct timespec);
void get_mac_from_interface(struct sockaddr_ll *);
void print_mac_addr(uint8_t *, size_t);
int send_raw_packet(int, struct sockaddr_ll *, uint8_t *, size_t);
int recv_raw_packet(int, uint8_t *, size_t);

#endif
