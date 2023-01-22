# LD2410 mmwave Motion detection
LD2410 mmwave Motion detection in ESPHome for Home Assistant . Can be combined with temperature, humidity and lux sensor  
sensor: https://nl.aliexpress.com/item/1005004786874722.html?spm=a2g0o.order_list.order_list_main.83.7b9c79d21Hc0g0&gatewayAdapt=Pc2Msite
  
other sources used  
https://community.home-assistant.io/t/mmwave-wars-one-sensor-module-to-rule-them-all/453260/2
  
https://github.com/rain931215/ESPHome-LD2410
  
### Step 1
copy the code from ld2410_uart.h into a new file in the esphome directory, using for example Studio Code server

### Step 2
Choose the ESP32 or ESP8266 code for your board  
![image](https://user-images.githubusercontent.com/100353268/213939599-cc16b760-055d-4786-9fc2-663132c9dd59.png)

#### Pinout ESP32 (ESP32-S2)
esphome example code: esp-motion-kinderkamer-lucas.yaml  
ESP32-S2 | LD2410  
5V <-> VCC  
GND <-> GND  
GPIO18 <-> RX  
GPIO33 <-> TX  
GPIO5 <-> OUT  

#### Pinout ESP8266 (Wemos d1 mini) Board
esphome example code: esp-motion-toilet.yaml  
ESP8266 | LD2410  
5V <-> VCC  
GND <-> GND  
TX <-> RX  
RX <-> TX  
D7 <-> OUT  
  
#### Example including ESP32-S2 + DHT11 + Lux sensor  
esphome example code: esp-motion-kinderkamer-sophie.yaml  

#### Step 3
Make sure you change all he !secret values, or create them in your esphome secrets  
Change at least the following (example)  
file: esp-motion-kinderkamer-lucas.yaml  
- name of the board you use
- names of the board in esphome and the motion sensor name (around line 166)

#### Step 4
Add to code to a new device in esphome  
Once finsihed and sensor is online, you can add it in Home assistant or visit the webpage of the device.
