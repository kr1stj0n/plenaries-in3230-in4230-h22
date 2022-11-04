#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <ifaddrs.h>	/* getifaddrs */

double diff_time_ms(struct timespec start, struct timespec end)
{
	double s, ms, ns;

	s  = (double)end.tv_sec  - (double)start.tv_sec;
	ns = (double)end.tv_nsec - (double)start.tv_nsec;

	if (ns < 0) { // clock underflow
		--s;
		ns += 1000000000;
	}

	ms = ((s) * 1000 + ns/1000000.0);

	return ms;
}

/*
 * Print MAC address in hex format
 */
void print_mac_addr(uint8_t *addr, size_t len)
{
        size_t i;

        for (i = 0; i < len - 1; i++) {
                printf("%02x:", addr[i]);
        }
        printf("%02x\n", addr[i]);
}

/*
 * This function gets a pointer to a struct sockaddr_ll
 * and fills it with necessary address info from the interface device.
 */
void get_mac_from_interface(struct sockaddr_ll *so_name)
{
        struct ifaddrs *ifaces, *ifp;
        /* Enumerate interfaces: */
        /* Note in man getifaddrs that this function dynamically allocates
           memory. It becomes our responsability to free it! */
        if (getifaddrs(&ifaces)) {
                perror("getifaddrs");
                exit(-1);
        }

        /* Walk the list looking for ifaces interesting to us */
        for (ifp = ifaces; ifp != NULL; ifp = ifp->ifa_next) {
                /* We make certain that the ifa_addr member is actually set: */
                if (ifp->ifa_addr != NULL &&
                    ifp->ifa_addr->sa_family == AF_PACKET &&
                    (strcmp("lo", ifp->ifa_name)))
                        /* Copy the address info into our temp. variable */
                        memcpy(so_name,
                               (struct sockaddr_ll*)ifp->ifa_addr,
                               sizeof(struct sockaddr_ll));
        }
        /* After the loop, the address info of the last interface
           enumerated is stored in so_name. */

        /* Free the interface list */
        freeifaddrs(ifaces);

        return;
}

/*
 * Send packet over the RAW socket
 */
int send_raw_packet(int sd, struct sockaddr_ll *so_name, uint8_t *buf, size_t len)
{
        struct ether_frame frame_hdr;
        struct msghdr      *msg;
        struct iovec       msgvec[2];
        int                rc;

        /* Fill in Ethernet header */
        /* The dst MAC address is hardcoded; we will use MIP-ARP in Home Exams */
        uint8_t dst_addr[] = BROADCAST_MAC_ADDR;
        memcpy(frame_hdr.dst_addr, dst_addr, 6);
        memcpy(frame_hdr.src_addr, so_name->sll_addr, 6);
        /* Match the ethertype in packet_socket.c: */
        frame_hdr.eth_proto[0] = frame_hdr.eth_proto[1] = 0xFF;

        /* Point to frame header */
        msgvec[0].iov_base = &frame_hdr;
        msgvec[0].iov_len  = sizeof(struct ether_frame);
        /* Point to frame payload */
        msgvec[1].iov_base = buf;
        msgvec[1].iov_len  = len;

        /* Allocate a zeroed-out message info struct */
        msg = (struct msghdr *)calloc(1, sizeof(struct msghdr));

        /* Fill out message metadata struct */
        msg->msg_name    = so_name;
        msg->msg_namelen = sizeof(struct sockaddr_ll);
        msg->msg_iovlen  = 2;
        msg->msg_iov     = msgvec;

        printf("\nSending a broadcast hello ...\n");
    
        /* Construct and send message */
        rc = sendmsg(sd, msg, 0);
        if (rc == -1) {
                perror("sendmsg");
                free(msg);
                return 1;
        }

        /* Remember that we allocated this on the heap; free it */
        free(msg);

        return rc;
}

/*
 * Receive packet from the RAW socket
 */
int recv_raw_packet(int sd, uint8_t *buf, size_t len)
{
        struct sockaddr_ll so_name;
        struct ether_frame frame_hdr;
        struct msghdr      msg;
        struct iovec       msgvec[2];
        int                rc;

        /* Point to frame header */
        msgvec[0].iov_base = &frame_hdr;
        msgvec[0].iov_len  = sizeof(struct ether_frame);
        /* Point to frame payload */
        msgvec[1].iov_base = buf;
        msgvec[1].iov_len  = len;


        /* Fill out message metadata struct */
        msg.msg_name    = &so_name;
        msg.msg_namelen = sizeof(struct sockaddr_ll);
        msg.msg_iovlen  = 2;
        msg.msg_iov     = msgvec;

        rc = recvmsg(sd, &msg, 0);
        if (rc == -1) {
                perror("sendmsg");
                return -1;
        }

        printf("neighbor with MAC address ");
        print_mac_addr(frame_hdr.src_addr, 6);
        printf(" sent: %s\n", buf);

        return rc;
}
