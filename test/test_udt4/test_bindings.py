#!/usr/bin/env python

import pyudt

import os, sys
sys.path.append(os.path.join(os.path.dirname(__file__)))

e = pyudt.Epoll()
print e
print e.id()
e.id(2)
print e.id()
s = pyudt.Socket()
print s
s.type(3)
print s
e.add_usock(s,0)
e.remove_usock(s)
