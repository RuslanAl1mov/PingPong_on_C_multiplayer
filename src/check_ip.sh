#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <IP_Address>"
    exit 1
fi

ip_address=$1

ping -c 1 -W 1 $ip_address > /dev/null

if [ $? -eq 0 ]; then
    exit 0
else
    exit 1
fi