#!/bin/bash

while true
do
    curl -X POST -H "Broker-Token: minhtoan" -d "hello world" "http://127.0.0.1:8080/push?app_name=test_app&topic_name=test_topic"
done
