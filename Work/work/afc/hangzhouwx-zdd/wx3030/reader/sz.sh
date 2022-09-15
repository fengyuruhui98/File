#!/bin/sh

echo "find /hhjt/reader -type d|grep  prog|xargs rm -rf"
find /hhjt/reader -type d|grep  prog|xargs rm -rf
echo "cd /hhjt/reader/current"
cd /hhjt/reader/current

echo "export LD_LIBRARY_PATH=/hhjt/reader/current:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH=/hhjt/reader/current:$LD_LIBRARY_PATH

echo "./tpmain > /dev/null &"
./tpmain > /dev/null &
