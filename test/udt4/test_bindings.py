#!/usr/bin/env python

import pyudt
from threading import Thread
import time

test_data='word'
test_data_len=len(test_data)

def threaded_client():
    # Create a UDT socket to simulate a client
    s_client = pyudt.Socket()
    s_client.connect('127.0.0.1',4444)

    # Send data to the server
    print 'Sending message: ' + str(test_data)
    s_client.send(test_data, test_data_len)

    # Close sockets
    s_client.close()

def threaded_server():

    # Create a UDT socket to simulate a server
    s_server = pyudt.Socket()
    s_server.bind('127.0.0.1', 4444)
    s_server.listen(100)

    # Create an epoll
    e = pyudt.Epoll()

    # Accept incoming connections
    client,addr_ip,addr_port = s_server.accept()

    # Add the client sockets to the epoll
    e.add_usock(client, pyudt.UDT_EPOLL_IN)

    # Ask the epoll to wait for events for 10ms
    e.wait(10, True, False, True, False)

    # Receive data from the client
    test_data_rcv = client.recv(test_data_len)
    print 'Received message: ' + str(test_data_rcv[0:test_data_len])

    # Garbage collect closed/broken sockets
    #e.garbage_collect()

    # Remove client socket from epoll
    e.remove_usock(client)

    # Close server socket
    s_server.close()

# ---------------------------------------------------------------------------- #

# Initialize the console logger
pyudt.Logger.init_logger()

# Startup UDT
pyudt.startup()

# Launch server and client threads
t_server = Thread(target = threaded_server, args = ())
t_client = Thread(target = threaded_client, args = ())

t_server.start()
t_client.start()

t_client.join()
t_server.join()

# Cleanup UDT
pyudt.cleanup()
