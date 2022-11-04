# 7th plenary session: 28.10.2021 #

In this plenary session we focused on the following topics:

* A queue library using doubly linked lists
* Finite State Machine

## libQueue ##

One requirement for the MIP and Routing daemon is that they MUST operate
asynchronously, which means that they must not block the execution flow while
waiting for response packets. In order to achieve this, one may combine several
methods like non-blocking sockets, epoll() or queues.
[Here](https://github.com/kr1stj0n/plenaries-in3230-in4230-h22/tree/main/p7_28-10-2022/libqueue)
you may find a complete implementation of a queue library that can be used in
your Home Exams. When the MIP daemon receives a MIP packet that needs to be
forwarded, it can store the packet in the queue, initiate a route lookup using
the forwarding engine, and then retrieve that packet from the queue. At the same
time, MIP daemon MUST be able to process incoming and send outgoing MIP packets.

## FSM (Finite State Machine) ##

A Routing daemon that runs a Routing Protocol can be modeled as a Finite State
Machine. The FSM can change from one state to another in response to some
inputs; the change from one state to another is called a transition. Examples of
transitions in our Home Exam 2 are when a HELLO message is sent to all neighbors
or when a neighbor is down and the Routing table needs to be updated. In this
example
[here](https://github.com/kr1stj0n/plenaries-in3230-in4230-h22/tree/main/p7_28-10-2022/fsm),
we implement a simple FSM that keeps track of the states between the client and
server. Both the client and server send HELLO messages every 5 seconds. HELLO
timeout is set 10 seconds. Using the mininet script, we introduce a delay of 2s
(RTT 4s) and loss (33%) in the point-to-poing link. You should consider adding a
delay and loss in your Home Exam to properly test how your routing algorithm
reconverges in case of dead neighbors or link failures.

Usage:

  `make all` to compile the programs.

Run the receiver first using `./receiver`, and then `./sender` to run the sender
in the other node.
