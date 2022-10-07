# 4th Plenary session (IN3230/4230)

In this example, we're going to implement a simple greeting protocol called
HiP - Hi Protocol.

## "HiP"

`topo_p2p.py` is the python script that generates the following mininet topology..

    [NodeA]ifAB------------ifBA[NodeB]

NodeA will send a broadcast HiP packet via the RAW socket and NodeB will reply
via a unicast HiP packet.

Usage:

- Compile all programmes with `make all` in the current directory
- Create the mininet topology using `sudo mn --custom topo_p2p.py --topo mytopo`
- In the mininet console, access node A and B using `xterm A B`
  (You should have used -Y argument in the SSH command:
  `ssh -Y debian@ip_address_of_your_VM`
- From the xterm consoles, run `./hip s` at node B and `./hip c hello` at node A.

  Practise and implement new features in the applications.

  Good luck :-)
