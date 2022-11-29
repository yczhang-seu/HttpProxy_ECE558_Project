#!/bin/bash
make clean
make all
echo 'start running proxy server...'
./main LFU &
while true ; do continue ; done
