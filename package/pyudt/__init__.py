
"""
:module: __init__.py

--------------------------------------------------------------------------------
Namespace: udt4
--------------------------------------------------------------------------------
udt4
    |
    +-- .UDTSOCKET          : Raw UDTSOCKET reference
    +-- .UDTepoll           : epoll implementation accepting UDTSOCKET instances
    +-- .UDTException       : Maps to UDTEXCEPTION (error_code, error_string)
    +-- .TRACEINFO          : Direct structure map to UDT::TRACEINFO
    |
    .pyudt
         |
         +-- .UdtSocket()   : Python style socket wrapper around UDTSOCKET
         |
         +-- .Epoll()       : Epoll implementation to be used with UdtSocket()

--------------------------------------------------------------------------------
Versioning:
Releases are done in {MAJOR}.{MINOR} of py-udt version.  All function
signatures are back-compatible on major and minor releases.

If and when UDT releases udt5, the py-udt library will be ported to v5, and
the py-udt4 will remain supported independently.

--------------------------------------------------------------------------------
License:
UDT4 library and py-udt4 wrapper is licensed under BSD.

--------------------------------------------------------------------------------
"""

__author__  = 'Christopher J. Hanks <develop@cjhanks.name>'
__date__    = '12/18/2012'
__license__ = 'GPLv3'


import os

import udt4_ext
from   udt4_ext import *
