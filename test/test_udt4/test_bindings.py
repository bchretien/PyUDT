#!/usr/bin/env python

import pyudt

import os, sys
sys.path.append(os.path.join(os.path.dirname(__file__)))

# Initialize the console logger
pyudt.Logger.init_logger()

# Create an epoll
e = pyudt.Epoll()

# Create a UDT socket
s = pyudt.Socket()
print s

# Add the socket to the epoll
e.add_usock(s)

# Ask the epoll to wait for events for 10ms
e.wait(10)

# Remove the socket from the epoll
e.remove_usock(s)

# Garbage collect closed/broken sockets
e.garbage_collect()
