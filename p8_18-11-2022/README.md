# 8th plenary session: 18.11.2022 #

In this plenary session we will talk about BIRD, the BIRD Internet Routing Daemon,
which was developed as a school project at Faculty of Math and Physics, Charles
University Prague.

## BIRD ##

The BIRD project aims to develop a fully functional dynamic IP routing daemon
primarily targeted on (but not limited to) Linux, FreeBSD and other UNIX-like
systems and distributed under the GNU General Public License. Read more about
BIRD [here](https://bird.network.cz/).  


Using
[this](https://github.com/kr1stj0n/plenaries-in3230-in4230-h22/blob/main/p8_18-11-2022/init_mn.py)
mininet script, we build a BIRD topology as shown in
[this](https://github.com/kr1stj0n/plenaries-in3230-in4230-h22/blob/main/p8_18-11-2022/bird_topology.pdf)
figure.  

### Installation ###

To install `bird` in the NREC virtual machines use `sudo apt install bird`
command, which will install `BIRD version 1.6.8`. The config files for r1, r2,
and r3 are written for this version.  

### Usage ###

To run the mininet script use the following command:

```
   $ sudo -E python3 init_mn.py
```

Inside the mininet CLI, launch xterms of the nodes with `xterm A B r1 r2 r3`.
Inside the xterms, one can connect to the BIRD controller using `birdc -s
bird.ctl` in the r1, r2, or r3 directories and apply Cisco-like commands (`show
routes`, `show rip/ospf neighbors`, etc.).  

Use `wireshark`, `tshark` or `tcpdump` to capture HELLO and UPDATEs packets of
the routing protocols.  

When the routing protocol has converged, you should be able to ping from A to B
`ping 162.168.3.100`.  

Last but not least, check the BIRD source code from its
[GitLab](https://gitlab.nic.cz/labs/bird) repository and get some inpiration for
your MIP routing daemon.  

Have a nice weekend and see you next Friday!
