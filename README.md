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

- Voltage: RMS, pk-pk, FFT, quality, indicate peak load times
- Current: RMS, pk-pk, FFT, usage, indicate internal load
- Power: inst., apparent, true, bill usage rate monthly, sense device
- Sense: temp., humidity, VOC, CO, CO2, PM, electric sound (devices)
- Control: room supply, mode setup
- More: state, district bill rate chart, predict bill, zero usage
- More: incentive non peak load, new home device recommendations
- More: shared info distribution, zero energy, device efficiency

Prepare features:
- Calculate time by fetching from NTP
- Show energy use graph
- Predict energy use based on current usage
- Predict bill based on current use
- Server size storage and analytics
- Enable configuration, price chart fetching
- Indicate energy source (coal, nuclear)
- Indicate savings
- Multiple access passwords
- Skill to ask for energy bill (ga, alexa)
- Previous month electricity bill, usage
- Prepaid mode?
- Reading averager
- Use watchdog to make more self resets
- Throughout day energy use
- Room wise energy use (/ac)
- Instruct fridge, ac?
- Hacker prevention



## device

- NVS partition generation


### http

- To write http apis here


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
