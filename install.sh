#!/bin/bash

if [ "$EUID" -ne 0 ]; then
    echo 'please run script as root'
    exit
fi

gcc -o /usr/bin/thingbrok thingbrok.c

mkdir -p /var/lib/thingbrok/databases/test_app/test_topic
