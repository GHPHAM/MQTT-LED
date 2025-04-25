Compile toggle executable with:
```
gcc -o MQTTc.exe MQTTcontroller.c -m64
```
To enable timed light toggle service:
* sudo systemctl enable mqtt-light-toggle.timer
* sudo systemctl start mqtt-light-toggle.timer

To disable timed light toggle service:
* sudo systemctl stop mqtt-light-toggle.timer

pre-requisite:
* PubSubClient for Arduino
* Mosquitto
