**Thingbrok - An broker for applications**

***What is Thingbrok?***  
* Thingbrok is an socket server stand between application back-end and front-end.  
* Thingbrok current work in HTTP protocol  

***Thingbrok model***  
```text
+---------------+          +-------------+          +--------------+  
|   End user    | <------> |  Thingbrok  | <------> |   Back end   |  
|  Application  |    API   |             |    API   |  Application |  
+---------------+          +-------------+          +--------------+ 
```

***Usage***
```bash
git clone https://github.com/pmtoan/thingbrok.git  
cd thingbrok  
gcc -o thingbrok thingbrok.c  
./thingbrok --help
``` 

***Example***
* Push data to thingbrok  
```bash
curl -X POST -H "Broker-Token: simple_key" -d "hello world" "http://127.0.0.1:8080/push?app_name=test_app&topic_name=test_topic"
```

* Pull data from thingbrok
```bash
curl -X GET -H "Broker-Token: simple_key" "http://127.0.0.1:8080/pull?app_name=test_app&topic_name=test_topic"
```


