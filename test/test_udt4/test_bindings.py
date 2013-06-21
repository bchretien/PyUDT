#!/usr/bin/env python

import pyudt

import struct
import os, sys
sys.path.append(os.path.join(os.path.dirname(__file__)))

# Initialize the console logger
pyudt.Logger.init_logger()

# Startup UDT
pyudt.startup()

# Create an epoll
e = pyudt.Epoll()

# Create a UDT socket to simulate a server
s_server = pyudt.Socket()
s_server.bind('127.0.0.1', 4444)
s_server.listen(100)
print s_server

# Create a UDT socket to simulate a client
s_client = pyudt.Socket()
s_client.bind('127.0.0.1', 4444)
s_client.connect('127.0.0.1',4444)
print s_client

# Add the sockets to the epoll
e.add_usock(s_client)
e.add_usock(s_server)

# Send data to the server
data_client = struct.pack('i', 10)
s_client.send(data_client,1)

# Receive data from the client
integer = s_server.recv(4)
print integer

# Ask the epoll to wait for events for 10ms
e.wait(10)

# Garbage collect closed/broken sockets
e.garbage_collect()

# Close sockets
s_client.close()
s_server.close()

# Cleanup UDT
pyudt.cleanup()
