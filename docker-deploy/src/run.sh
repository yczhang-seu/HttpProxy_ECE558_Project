#!/bin/bash
make clean
make all
echo 'start running proxy server...'
./main LRU &
while true ; do continue ; done
