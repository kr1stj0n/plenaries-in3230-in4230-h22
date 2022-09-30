# 3rd Plenary session (IN3230/4230) - RAW sockets (continued)

In this example, we are going to implement a simple ARP protocol which will help
two nodes to get to know the neighbor between them.

## "Greet your neighbor!"

`topo_p2p.py` is the python script that generates the mininet topology, which
consists of three nodes, A, B and C.

[NodeA]ifAB------------ifBA[NodeB]ifBC------------ifCB[NodeC]

NodeA and NodeC will talk to NodeB via RAW sockets. `epoll()` will be used to
monitor the activity of the RAW socket descriptors.

While NodeB will be listening to a RAW socket, NodeA/NodeC will send a broadcast
frame asking "Tell me who are you?". NodeB will reply with its MAC address via
the interface where the request came from, and will cache NodeA/NodeC's MAC
address.

Usage:

- Compile the project running `make all` in the current directory
- Create the mininet topology using `sudo mn --custom topo_p2p.py --topo mytopo`
- In the mininet console, access node A, B, C using `xterm A B C`
  (You should have used -Y argument in the SSH command:
  `ssh -Y debian@ip_address_of_your_VM`
- From the xterm consoles, run `./nodeB` at node B and `./nodeA` at node A or
  `./nodeC` at node C.

Practise and implement new features in the applications.

Good luck :-)
