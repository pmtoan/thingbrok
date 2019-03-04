#!/bin/bash

if [ "$EUID" -ne 0 ]; then
    echo 'please run script as root'
    exit
fi

gcc -std=gnu99 -o /usr/bin/thingbrok thingbrok.c
gcc -std=gnu99 -o /usr/bin/thingbrokctl thingbrokctl.c

mkdir -p /var/lib/thingbrok/databases/test_app/test_topic
