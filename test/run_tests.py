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
        self.add_usock_flags()
        self.remove_usock()
        self.add_ssock()
        self.add_ssock_flags()
        self.remove_ssock()
        self.get_id()
        self.garbage_collect()
        self.get_read_udt()

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

    def add_usock_flags(self):
        epoll = pyudt.Epoll()
        socket = pyudt.Socket()
        try:
            epoll.add_usock(socket, pyudt.UDT_EPOLL_IN)
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
            epoll.add_ssock(socket)
        except:
            self.fail('Error in Epoll.add_ssock')

    def add_ssock_flags(self):
        epoll = pyudt.Epoll()
        socket = socklib.socket()
        try:
            epoll.add_ssock(socket, pyudt.UDT_EPOLL_IN)
        except:
            self.fail('Error in Epoll.add_ssock')

    def remove_ssock(self):
        epoll = pyudt.Epoll()
        socket = socklib.socket()
        try:
            epoll.add_ssock(socket)
            epoll.remove_ssock(socket)
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

    def get_read_udt(self):
        epoll = pyudt.Epoll()
        socket = pyudt.Socket()
        epoll.add_usock(socket)
        try:
            res = epoll.get_read_udt()
        except:
            self.fail('Error in Epoll.get_read_udt')

# Run unit tests
if __name__ == '__main__':
    unittest.main()
