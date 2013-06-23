#!/usr/bin/env python

import unittest
import pyudt

# Test fixture for the Socket class
class SocketTest(unittest.TestCase):
    def runTest(self):
	self.creation()
	self.destruction()

    def creation(self):
	socket = pyudt.Socket()
	assert socket != None

    def destruction(self):
	socket = pyudt.Socket()
	try:
	    del socket
	except:
	    self.fail('Error during socket destruction')

# Test fixture for the Epoll class
class EpollTest(unittest.TestCase):
    def runTest(self):
	self.creation()
	self.destruction()
	self.add_usock()
	self.remove_usock()

    def creation(self):
	epoll = pyudt.Epoll()
	assert epoll != None

    def destruction(self):
	epoll = pyudt.Epoll()
	try:
	    del epoll
	except:
	    self.fail('Error during epoll destruction')

    def add_usock(self):
	epoll = pyudt.Epoll()
	socket = pyudt.Socket()
	try:
	    epoll.add_usock(socket)
	except:
	    self.fail('Error in Epoll.add_usock')

    def remove_usock(self):
	epoll = pyudt.Epoll()
	socket = pyudt.Socket()
	try:
	    epoll.add_usock(socket)
	    epoll.remove_usock(socket)
	except:
	    self.fail('Error in Epoll.add_usock')

# Run unit tests
if __name__ == '__main__':
    unittest.main()
