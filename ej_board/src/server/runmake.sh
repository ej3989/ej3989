#!/bin/bash
cd ../
make clean
make
cd server/
./server_main
