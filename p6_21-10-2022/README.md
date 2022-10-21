# 6th plenary session: 21.10.2022 #

In this sessions we focused on Home Exam 2 by covering most of the new concepts
that it introduces. In Home Exam 2 we will extend our network stack that we are
building in this course by implementing a routing daemon, which will provide
forwarding and routing services to the MIP daemon.  

Using a similar topology as in Home Exam 2, we demonstrated step by step how a
routing daemon can:

- discover neighbors using HELLO messages
- build a routing table and share its routes by UPDATE messages
- detect failures in the network

The routing daemon should run a simple Distance Vector Routing (DVR) protocol,
with Poisoned Reverse loop protection. An example of DVR protocol is RIPv1,
which due to its limitations is superseeded by RIPv2.

Read more about RIP (Routing Information Protocol) [here](https://en.wikipedia.org/wiki/Routing_Information_Protocol).
