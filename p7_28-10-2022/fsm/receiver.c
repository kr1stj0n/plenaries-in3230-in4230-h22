#include <stdint.h>
#include <stdio.h> 		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* close */
#include <string.h>		/* memset */
#include <sys/socket.h>		/* socket */
#include <sys/epoll.h>          /* epoll */
#include <arpa/inet.h>		/* htons */

#include "common.h"

#define BUF_SIZE 1450

int main(int argc, char *argv[])
{
        struct    sockaddr_ll so_name;
        uint8_t   buf[BUF_SIZE];
        int       raw_sock, rc;

        struct epoll_event ev, events[MAX_EVENTS];
        int epollfd;

        short unsigned int protocol = 0xFFFF;

        enum state s_state;
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


        /* Fill the fields of so_name with info from interface */
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

        printf("Waiting for hello ...\n");
        /* Receiver waits forever for the first hello and then enters into an
         * INIT state */
        rc = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (rc == -1) {
                perror("epoll_wait");
                exit(EXIT_FAILURE);
        } else {
                printf("Received the first hello ... entering INIT state\n");
                s_state = INIT;
                clock_gettime(CLOCK_REALTIME, &lastHelloRecv);
        }
        
        while (1) {
                switch (s_state) {
                case INIT:
                        /* Send hello to the neighbor. */
                        send_raw_packet(raw_sock,
                                        &so_name,
                                        hello,
                                        sizeof(hello));
			
                        clock_gettime(CLOCK_REALTIME, &lastHelloSent);
                        s_state = WAIT;
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
                                        s_state = EXIT;
                                        continue;
                                } else {
                                        if (diff_time_ms(lastHelloSent, timenow) >= HELLO_INTERVAL) {
                                                /* Time to send HELLO */
                                                s_state = INIT;
                                                continue;
                                        }
                                }
                        } else {
                                /* epoll() was triggered -> read the HELLO */
                                if (recv_raw_packet(raw_sock, buf, BUF_SIZE) <= 0)
                                        s_state = EXIT;
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
