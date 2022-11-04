""" Plenary IN3230/4230 - Point-to-Point topology

    host A <----> host B

    Usage: sudo mn --mac --custom topo_p2p.py --topo mytopo

"""

from mininet.topo import Topo

class MyTopo( Topo ):
    "Point to Point topology"

    def __init__( self ):
        "Create custom topo."

        # Initialize topology
        Topo.__init__( self )

        # Add hosts
        A = self.addHost( 'A' )
        B = self.addHost( 'B' )

        # Add link
        self.addLink(A, B, bw=10, delay='2s', loss=33.0, use_tbf=False)

topos = { 'mytopo': ( lambda: MyTopo() ) }
