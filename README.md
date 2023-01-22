# LD2410 mmwave Motion detection
LD2410 mmwave Motion detection in ESPHome for Home Assistant . Can be combined with temperature, humidity and lux sensor

# Step 1
copy the code from ld2410_uart.h into a new file in the esphome directory, using for example Studio Code server

# Step 2
Choose the ESP32 or ESP8266 code for your board  
![image](https://user-images.githubusercontent.com/100353268/213939599-cc16b760-055d-4786-9fc2-663132c9dd59.png)

## Pinout ESP32 (ESP32-S2)

esphome code: 

ESP32-S2 | LD2410  
![image](https://user-images.githubusercontent.com/100353268/213940296-db9187e2-653e-4eb4-b36b-328f2b424bdc.png)

5V <-> VCC  
GND <-> GND  
GPIO18 <-> RX  
GPIO33 <-> TX  
GPIO5 <-> OUT  

## Pinout ESP8266 (Wemos d1 mini) Board
esphome code: 

ESP8266 | LD2410
![image](https://user-images.githubusercontent.com/100353268/213940308-62fd8b25-efde-49a5-9247-694ea7481811.png)

5V <-> VCC
GND <-> GND
TX <-> RX
RX <-> TX
D7 <-> OUT

# Step 3
Make sure you change all he !secret values, or create them in your esphome secrets
Change at least the following (example)
file: esp-motion-test.yaml
- name (line 2)
- name (line 157)

# Step 4
Add to code to a new device in esphome
Once finsihed and sensor is online, you can add it in Home assistant or visit the webpage of the device.
