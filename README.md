# *ScreenTimer*
## **CASA0016 Course Work** 
### _A Device for Screen Usage Detection and Data Visualization_


In this project, I designed a **physical device** to **detect and record** the usage time of various screens in daily life. The primary goal is to **visually display** this data on an **interesting flip clock**.


<center>
  <img src="./Src/reset_demo.gif" alt="Overall Photo of Website and Plant Monitor" width="400">
</center>

The device consists of three components: the Video collection component, the data processing component, and the time display component.

## Parts List
### 1. Video Collection Component
**1.1 FREENOVE ESP32-WROVER CAM Board**

To minimize the size of our visual data collection device while ensuring essential functionality, we've integrated a compact camera module along with a wireless network connection module. For this purpose, we've selected the renowned and efficient ESP32 series microprocessor. This choice strikes an optimal balance between small form factor and robust performance, catering to our device's need for seamless image capture and effortless connectivity.

<center>
  <img src="./Src/ESP32_CAM.jpg" alt="Image" width="300">
</center>

## Folder Structure

```none
Screen Timer Project
├── Code (All the code for Arduino UNO, ESP32 and Rasberry Pi)
│   ├── Arduino (Code tested on Arduino UNO R4 Wifi)
│   │   ├── Final_Mqtt_MultiMotor (Integrated Code)
│   │   │   ├── Final_Mqtt_MultiMotor.ino (Main Part)
│   │   │   ├── wifiConfig.h (Create your own config file!!)
│   │   ├── Hall_Effect_Sensor_Test (Integrated Code)
│   │   │   ├── Final_Mqtt_MultiMotor.ino (Main Part)
│   ├── Test (I save the sperate module script to test different function)
│   │   │   ├── ConnectToWifi
│   │   │   │   ├── ConnectToWifi.ino
```

## Common Issues

**1. No moisture value**

<center>
  <img src="./Links/issue01.png" alt="DHT22 Temp-Humidity Sensor" width="400">
</center>


  This mostly result from a extremly large resistance between two nails. 
    
For example, when you test the code at first, you may hold the two respectively with your hands, the voltage read by the analog pin is too small to return.

**2. Rasberry Pi SSH Connection**

The format for ssh connection is 

<u>"ssh {rasp pi username}@{rasp pi hostname}.local"</u>

The password is set up behind the username.

## Current Output

- Data In CE-Lab MQTT Broker From My Own Topic
<center>
  <img src="./Links/output_mqtt.png" alt="DHT22 Temp-Humidity Sensor" width="400">
</center>

- Data In InfluxDB Dashboard
<center>
  <img src="./Links/InfluxDB_Output.png" alt="DHT22 Temp-Humidity Sensor" width="400">
</center>

- Data Visualization in Grafana Dashboard

<center>
  <img src="./Links/Grafana_Output.png" alt="DHT22 Temp-Humidity Sensor" width="400">
</center>


## Future

### 1. Build Website Interaction

Set up web connection to mqtt and interactive with a led light to remind people around the plant through sending message to a newly built topic.

Collect and visualize the data on the same website. Analyse the former status of plant, compute the last time someone watering plant. 

<center>
  <img src="./Links/Web Interaction.png" alt="DHT22 Temp-Humidity Sensor" width="300">
</center>

### 2. Change the Watering Method with automatic control

Switching from top watering to bottom watering and adding an automatic control module that allows people to operate it from the website above. 

Bottom watering can prevent the topsoil from becoming too compact, allowing water to flow smoothly and avoiding waterlogging, which can attract many mosquitoes.

<center>
  <img src="./Links/Watering Method.jpeg" alt="Bottom Watering" width="300">
</center>

### 3. Parametric model for 3d Enclosure

Estabilish a parameteric model that fits various pot sizes. With combination of 3D printing tech, we can finish the 3D enclosure models quickly.

The general workflow for the parametric model:

- Input: Port Shape, Port Size, Depth of the Gap

- Output: STL Format Model (For 3D Modelling)

A simple demo in Rhino.

<center>
  <img src="./Links/3D Enclosure.gif" alt="DHT22 Temp-Humidity Sensor" width="300">
</center>

## Lisence
