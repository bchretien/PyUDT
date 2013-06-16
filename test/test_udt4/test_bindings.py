#!/usr/bin/env python

import pyudt

import os, sys
sys.path.append(os.path.join(os.path.dirname(__file__)))

e = pyudt.Epoll()
print e
print e.getId

s = pyudt.Socket()
print s
s.setType(3)
print s
