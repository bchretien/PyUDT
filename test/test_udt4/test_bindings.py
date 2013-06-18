#!/usr/bin/env python

import pyudt

import os, sys
sys.path.append(os.path.join(os.path.dirname(__file__)))

# Create an epoll
e = pyudt.Epoll()
print e
print e.id()

# Create a UDT socket
s = pyudt.Socket()
print s

# Add the socket to the epoll
e.add_usock(s,0)

# Remove the socket from the epoll
e.remove_usock(s)
