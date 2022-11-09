#!/bin/bash
cd ../src
make clean
make
cd ../bin
./server_main
