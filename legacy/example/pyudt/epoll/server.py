#!/usr/bin/env python

"""
:module udtserver
"""

import struct
import udt4 as     udt
from   udt4 import pyudt
import socket as socklib
from   subprocess import Popen
import sys


def configure_epoll(udt_clients, sys_clients):
    epoll = pyudt.Epoll()

    for client in udt_clients:
        epoll.add_usock(client, udt.UDT_EPOLL_IN)

    for client in sys_clients:
        epoll.add_ssock(client, udt.UDT_EPOLL_IN)

    return epoll

def clean_epoll(epoll, udt_clients, sys_clients):
    for client in udt_clients:
        epoll.remove_usock(client, udt.UDT_EPOLL_IN)

    for client in sys_clients:
        epoll.remove_ssock(client, udt.UDT_EPOLL_IN)


def acquire_udt_clients(serv_addr, serv_port, count):
    print('Acquiring UDT clients')
    udt_serv = pyudt.UdtSocket()
    udt_serv.bind( (serv_addr, serv_port) )
    udt_serv.listen(100)

    clients = []

    for i in xrange(count):
        Popen('./client.py %(host)s %(protocol)s %(port)i ' % {
                'host' : serv_addr, 'port' : serv_port, 'protocol' : 'udt' },
              shell = True
              )

        socket, client_addr = udt_serv.accept()
        clients.append(socket)

    return udt_serv, clients


def acquire_tcp_clients(serv_addr, serv_port, count):
    print('Acquiring TCP clients')
    tcp_serv = socklib.socket()
    tcp_serv.bind( (serv_addr, serv_port) )
    tcp_serv.listen(100)

    clients = []

    for i in xrange(count):
        Popen('./client.py %(host)s %(protocol)s %(port)i ' % {
                'host' : serv_addr, 'port' : serv_port, 'protocol' : 'tcp' },
              shell = True
              )

        socket, client_addr = tcp_serv.accept()
        clients.append(socket)

    return tcp_serv, clients


def handle_socket_uset(epoll, sock_set):
    """
    Ideally this function should work for both UdtSocket type and socket.socket
    type.

    standard:
    4   byte - version
    8   byte - length
    k   byte - message
    """
    for sock in sock_set:
        integer = sock.recv(4)
        if integer:
          try:
              version = struct.unpack('i', integer)
              msg_len = struct.unpack('l', sock.recv(8))[0]
              msg     = sock.recv(msg_len)
          except:
              sys.stderr.write('UDT socket closed')
              epoll.remove_usock(sock)
              continue
          else:
              print('UDT message: ' + msg)

def handle_socket_sset(epoll, sock_set):
    """
    Ideally this function should work for both UdtSocket type and socket.socket
    type.

    standard:
    4   byte - version
    8   byte - length
    k   byte - message
    """
    for sock in sock_set:
        integer = sock.recv(4)
        if integer:
          try:
              version = struct.unpack('i', integer)
              msg_len = struct.unpack('l', sock.recv(8))[0]
              msg     = sock.recv(msg_len)
          except:
              sys.stderr.write('TCP socket closed')
              epoll.remove_ssock(sock)
              continue
          else:
              print('TCP message: ' + msg)

def main():
    """
    """
    udt.startup()

    udt_serv, udt_clients = acquire_udt_clients('127.0.0.1', 4001, 3)
    tcp_serv, tcp_clients = acquire_tcp_clients('127.0.0.1', 4002, 3)

    print('Configuring epoll')
    epoll = configure_epoll(udt_clients, tcp_clients)

    print('Reading messages')
    i = 0
    while True:
        sets = epoll.wait(True, False, 10000, True, False)

        handle_socket_uset(epoll, sets[0])
        handle_socket_uset(epoll, sets[1])
        handle_socket_sset(epoll, sets[2])
        handle_socket_sset(epoll, sets[3])

        if len(sets[0]) == 0:
            i += 1

        if i == 12:
            print('... done!')
            break

    print('Cleaning epoll')
    clean_epoll(epoll, udt_clients, tcp_clients)

    print('Closing sockets')
    for client in udt_clients:
        client.close()

    for client in tcp_clients:
        client.close()

    udt_serv.close()
    tcp_serv.close()

    udt.cleanup()


if __name__ == '__main__':
    from sys import exit
    exit(main())
