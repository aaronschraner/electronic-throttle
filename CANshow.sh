#!/bin/sh
#screen /dev/ttyACM1 115200
cu -l /dev/ttyACM1 -s 115200 | while read x;
do
    echo -n "Received can data: $x";
done
