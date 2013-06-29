#!/usr/bin/env python

import unittest
import pyudt
import socket as socklib

# Test fixture for the Socket class
class SocketTest(unittest.TestCase):
    def runTest(self):
        self.creation()
        self.destruction()
        self.close()
        self.descriptor()

    def creation(self):
        socket = pyudt.Socket()
        assert socket != None

    def destruction(self):
        socket = pyudt.Socket()
        try:
            del socket
        except:
            self.fail('Error during socket destruction')

    def close(self):
        socket = pyudt.Socket()
        try:
            socket.close()
        except:
            self.fail('Error during socket closure')

    def descriptor(self):
        socket = pyudt.Socket()
        assert socket.descriptor() != 0

# Test fixture for the Epoll class
class EpollTest(unittest.TestCase):
    def runTest(self):
        self.creation()
        self.destruction()
        self.add_usock()
        self.remove_usock()
        self.add_ssock()
        self.remove_ssock()
        self.get_id()
        self.garbage_collect()

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
            self.fail('Error in Epoll.remove_usock')

    def add_ssock(self):
        epoll = pyudt.Epoll()
        socket = socklib.socket()
        try:
            epoll.add_ssock(socket.fileno())
        except:
            self.fail('Error in Epoll.add_ssock')

    def remove_ssock(self):
        epoll = pyudt.Epoll()
        socket = socklib.socket()
        try:
            epoll.add_ssock(socket.fileno())
            epoll.remove_ssock(socket.fileno())
        except:
            self.fail('Error in Epoll.remove_ssock')

    def get_id(self):
        epoll = pyudt.Epoll()
        assert epoll.id() != 0

    def garbage_collect(self):
        epoll = pyudt.Epoll()
        try:
            epoll.garbage_collect()
        except:
            self.fail('Error in Epoll.garbage_collect')

# Run unit tests
if __name__ == '__main__':
    unittest.main()
