# ESP32-Smart-Monitoring-ThingSpeak
An IoT-based Smart Monitoring System using ESP32 that monitors light intensity, distance, and tilt orientation in real-time and visualizes data on a ThingSpeak cloud dashboard.
# IoT Smart Monitoring System using ESP32

This project demonstrates a complete IoT-based smart monitoring and automation system using ESP32.

## Features
- Ultrasonic distance monitoring
- Light intensity sensing using LDR
- Tilt detection using MPU6050
- Local automation using LED and Servo motor
- Real-time cloud data visualization using ThingSpeak

## Hardware Used
- ESP32 Development Board
- HC-SR04 Ultrasonic Sensor
- LDR + 10kΩ resistor
- MPU6050 / MPU6500
- Servo Motor (externally powered)
- LED + resistor

## Cloud Platform
- ThingSpeak (3 Fields)
  - Field 1: Distance (cm)
  - Field 2: Light Intensity
  - Field 3: Tilt Status

## Working
Sensor data is collected by ESP32 and uploaded to ThingSpeak every 15 seconds.  
Based on environmental conditions, the system performs local actions using LED and servo motor.

## Author
Nancy
