#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* close */
#include <string.h>		/* memset */
#include <sys/socket.h>		/* socket */
#include <fcntl.h>
#include <sys/epoll.h>		/* epoll */
#include <linux/if_packet.h>	/* AF_PACKET */
#include <arpa/inet.h>		/* htons */

#include "common.h"

#define BUF_SIZE 1450


int main(int argc, char *argv[])
{
        int     raw_sock, rc;
        struct  sockaddr_ll so_name;
        uint8_t buf[BUF_SIZE];

        short unsigned int protocol;
        protocol = 0xFFFF;

        struct epoll_event ev, events[MAX_EVENTS];
        int epollfd;

        enum state cln_state;
        struct timespec lastHelloSent;
        struct timespec lastHelloRecv;
        struct timespec timenow;
        uint8_t hello[6] = "hello";

        /* Set up a raw AF_PACKET socket without ethertype filtering */
        raw_sock = socket(AF_PACKET, SOCK_RAW, htons(protocol));
        if (raw_sock == -1) {
                perror("socket");
                return -1;
        }

        get_mac_from_interface(&so_name);

        epollfd = epoll_create1(0);
        if (epollfd == -1) {
                perror("epoll_create1");
                exit(EXIT_FAILURE);
        }

        ev.events = EPOLLIN;
        ev.data.fd = raw_sock;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, raw_sock, &ev) == -1) {
                perror("epoll_ctl: raw_sock");
                exit(EXIT_FAILURE);
        }

        cln_state = INIT;
        clock_gettime(CLOCK_REALTIME, &lastHelloRecv);

        while (1) {
                switch (cln_state) {
                case INIT:
                        /* Send hello to the neighbor. */
                        send_raw_packet(raw_sock,
                                        &so_name,
                                        hello,
                                        sizeof(hello));
                        
                        clock_gettime(CLOCK_REALTIME, &lastHelloSent);
                        cln_state = WAIT;
                        break;
                case WAIT:
                        rc = epoll_wait(epollfd, events, MAX_EVENTS, 1000);
                        if (rc == -1) {
                                perror("epoll_wait");
                                exit(EXIT_FAILURE);
                        } else if (rc == 0) {
                                clock_gettime(CLOCK_REALTIME, &timenow);
                                if (diff_time_ms(lastHelloRecv, timenow) > HELLO_TIMEOUT) {
                                        /* Timeout expired and didn't get any Hello */ 
                                        cln_state = EXIT;
                                        continue;
                                } else {
                                        if (diff_time_ms(lastHelloSent, timenow) >= HELLO_INTERVAL) {
                                                /* Time to send HELLO */
                                                cln_state = INIT;
                                                continue;
                                        }
                                }
                        } else {
                                /* epoll() was triggered -> read the HELLO */
                                if (recv_raw_packet(raw_sock, buf, BUF_SIZE) <= 0)
                                        cln_state = EXIT;
                                else {
                                        clock_gettime(CLOCK_REALTIME, &lastHelloRecv);
                                }
                        }
                        break;
                case EXIT:
                        close(raw_sock);
                        exit(EXIT_SUCCESS);
                        break;
                default:
                        // undefined state
                        exit(EXIT_FAILURE);
                        break;
                }
        }

        close(raw_sock);

        return 0;
}
