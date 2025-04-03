# LD2410 mmwave Motion detection
Now working with the native LD2410 component in Esphome!!  
LD2410 mmwave Motion detection in ESPHome for Home Assistant . Can be combined with temperature, humidity and lux sensor  
sensor: https://nl.aliexpress.com/item/1005004786874722.html?spm=a2g0o.order_list.order_list_main.83.7b9c79d21Hc0g0&gatewayAdapt=Pc2Msite
  
3D Printed box used: https://www.thingiverse.com/thing:5631878
  
### Step 1
Collect your Boards, sensors and code.  
The code uses the native LD2410 Esphome integration now, and does not need any additional files anymore.

### Step 2
Connect the hardware and get an usb cable for uploading firmware to your ESP32.  
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
![image](https://user-images.githubusercontent.com/100353268/213941685-f02bab19-3bf9-4c9e-8396-ed6582ae09ed.png)

![image](https://user-images.githubusercontent.com/100353268/213941779-5eb1bda0-0fc9-4edf-bc72-381e0cae259f.png)
![image](https://user-images.githubusercontent.com/100353268/213941793-3d5528d8-2f03-48c3-8c6d-71cf0ac28136.png)
![image](https://user-images.githubusercontent.com/100353268/213941799-9a551b3f-e476-4d73-b4c2-06db9502a43a.png)


#### Step 3
Choose the ESP32 code of your flavour (including or without other sensors)  

##### Example including ESP32 + LD2410  
example: esp-motion-toiet.yaml
##### Example including ESP32 + LD2410 + Dallas temp sensor + Lux sensor  
example: esp-motion-slaapkamer.yaml
##### Example including ESP32 + LD2410 + DHT11 (temp/hum) + Lux sensor  
example: esp-motion-kinderkamer-sophie.yaml  

Make sure you change all the !secret values, or create them in your esphome secrets  
Change all the advised fields. I marked them with a note (--> CHANGE to your own....)    

#### Step 4
Add to code to a new device in esphome  
Once finsihed and sensor is online, you can add it in Home assistant or visit the webpage of the device.

#### Home Assistant


#### Web Config



## other sources used  
https://community.home-assistant.io/t/mmwave-wars-one-sensor-module-to-rule-them-all/453260/2
  
https://github.com/rain931215/ESPHome-LD2410
  
