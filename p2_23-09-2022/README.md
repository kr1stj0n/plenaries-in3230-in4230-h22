# 2nd Plenary session: 23.09.2022 #

The second plenary session was focused on the basic concepts one needs to know
in order to start coding the MIP daemon and Ping applications of Home Exam 1.
The concepts include the following:

* UNIX sockets
* epoll()
* RAW sockets

## UNIX sockets ##

We first talked about UNIX sockets and checked the implementation of a
client/server chat application that uses such sockets to send data. UNIX sockets
will be use as the interface between the Ping applications and MIP daemons.
Check the code example
[here](https://github.com/kr1stj0n/plenaries-in3230-in4230-h21/tree/main/p2_23-09-2022/unix_sockets).

## epoll() ##

Then we talked about `epoll`, the great tool which monitors
multiple file descriptors to see if `read()` can be called on any of them.
`epoll()` will be used heavily in the Home Exams so that the daemons
can monitor both UNIX and RAW sockets for incoming data, asynchronously.

Here is an example from `man epoll`:

```c
#define MAX_EVENTS 10
           struct epoll_event ev, events[MAX_EVENTS];
           int listen_sock, conn_sock, nfds, epollfd;

           /* Code to set up listening socket, 'listen_sock',
              (socket(), bind(), listen()) omitted. */

           epollfd = epoll_create1(0);
           if (epollfd == -1) {
               perror("epoll_create1");
               exit(EXIT_FAILURE);
           }

           ev.events = EPOLLIN;
           ev.data.fd = listen_sock;
           if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
               perror("epoll_ctl: listen_sock");
               exit(EXIT_FAILURE);
           }

           for (;;) {
               nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
               if (nfds == -1) {
                   perror("epoll_wait");
                   exit(EXIT_FAILURE);
               }

               for (n = 0; n < nfds; ++n) {
                   if (events[n].data.fd == listen_sock) {
                       conn_sock = accept(listen_sock,
                                          (struct sockaddr *) &addr, &addrlen);
                       if (conn_sock == -1) {
                           perror("accept");
                           exit(EXIT_FAILURE);
                       }
                       ev.events = EPOLLIN | EPOLLET;
                       ev.data.fd = conn_sock;
                       if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                                   &ev) == -1) {
                           perror("epoll_ctl: conn_sock");
                           exit(EXIT_FAILURE);
                       }
                   } else {
                       do_use_fd(events[n].data.fd);
                   }
               }
           }
```

## RAW sockets ##

Last but not least, we talked about RAW sockets and how to send and receive
packets over them.
[Here](https://github.com/kr1stj0n/plenaries-in3230-in4230-h22/tree/main/p2_23-09-2022/raw_sockets)
you may find the source code of a sender and receiver application that send data
via RAW sockets. The code contains implementations of some utility functions
such as `print_mac_addr()`, `get_mac_from_interface()`, and
`send/recv_raw_packets()` that you can use in your own implementation of the MIP
daemon.

## Next plenary ##

If today's plenary might have been a bit confusing for some of you due to new
the new concepts, fear not nor be discouraged. In the next plenary session, we
will code a simple implementation of MIP-ARP using RAW socket, that helps three
nodes (A --- B --- C) to get to know each other. MIP-ARP is a crucial part of
the MIP daemon of Home Exam 1. Meanwhile, consider to check
<https://beej.us/guide/bgnet/>, which is a good resource about socket
programming in C.</br>
</br>
Have a nice weekend and see you next Friday!
