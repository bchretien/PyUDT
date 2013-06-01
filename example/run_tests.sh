#!/bin/bash

set -e

echo
echo "#############################################"
echo "#            RUNNING UDT4 TESTS             #"
echo "#############################################"
cd udt4 

cd mixed_python_c
echo
echo "-------------------------------"
echo "--- TESTING LANGUAGE MIXING ---"
echo "-------------------------------"
echo
./build.sh
./run_tests.sh
cd ..

cd sendrecv_file
echo
echo "------------------------------"
echo "--- TESTING SEND/RECV FILE ---"
echo "------------------------------"
echo
./init.py
cd ..

cd socket
echo
echo "-----------------------------"
echo "--- TESTING DGRAM SENDING ---"
echo "-----------------------------"
echo
./init.py DGRAM

echo
echo "-----------------------------------"
echo "--- TESTING SOCK_STREAM SENDING ---"
echo "-----------------------------------"
echo
./init.py NOT_DGRAM
cd ..

cd ..

echo
echo "#############################################"
echo "#            RUNNING PYUDT TESTS            #"
echo "#############################################"

cd pyudt

cd epoll
echo
echo "-----------------------------"
echo "--- TESTING pyudt.Epoll() ---"
echo "-----------------------------"
echo
./server.py
cd ..

cd perfmon
echo
echo "----------------------------------------"
echo "--- TESTING pyudt.perfmon and socket ---"
echo "----------------------------------------"
echo
./server.py
cd ..

cd settings
echo
echo "---------------------------------------------"
echo "--- TESTING pyudt.UdtSocket() and options ---"
echo "---------------------------------------------"
echo
./server.py
cd ..

cd fail0
echo
echo "---------------------"
echo "--- TESTING fail0 ---"
echo "---------------------"
echo
./test.py
cd ..

cd ..
echo
echo "All tests passed!"
