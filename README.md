ESP32 code for 3-phase realtime energy metering device to MQTT server through WiFi.

```text
includePath:
/home/wolfram77/esp/esp-idf/components/**
```



## situation

- World population growing, moving to cities
- Urban density & complexity increasing
- Energy consumption increasing substatially
- Climate is changing, prepare for future climates
- Slow change in developed countries
- Buildings are becoming complex
- Building integrated photovoltaics
- Sustainable built environment
- Higher comfort demands
- Healthy buildings



## conserve

- Natural lighting & ventilation
- Upgrade to energy efficient appliances
- Device usage statistics (geyser, fridge, AC)
- Reduce reliance on appliances (candlelight, water cooler)
- Unplug devices when not in use
- Batch tasks (baking all in one go)
- Use renewable energy



## features



## device

- NVS partition generation


### reference

```text
ESP32  --->  WiFi     BT    2 cores  peripherals
(40mm)     (2.4Ghz)  (4.2)  (+ULP)
```

[ESP-IDF](https://github.com/espressif/esp-idf)


### setup

```bash
# enable access to serial port (TTY)
sudo usermod -a -G dialout $USER

# environment variables
$ESPPORT, $ESPBAUD, $MONITORBAUD
```


### flash

```bash
make menuconfig     # application configuration
make -j8            # build application
make flash          # flash to device
make monitor        # monitor device logs
```


## mqtt

```bash
# kill process using a port
lsof -i :1883
kill -9 <pid>

# start MQTT server and client
mosquitto
mosquitto_sub -h test.mosquitto.org -t "#" -v
```
