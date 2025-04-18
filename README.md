# ESP8266 Sensor Project

This project collects sensor data (light, temperature, humidity) and sends it to configurable server endpoints. It’s set up for PlatformIO in VSCode, with an alternative Arduino IDE configuration available.

## Features
- Sensor data collection
- HTTP POST to server endpoints
- Uses: ESP8266WiFi, ESP8266HTTPClient, OneWire, DallasTemperature, DHT, Adafruit_NeoPixel

## Prerequisites
- PlatformIO Extension (VSCode) or Arduino IDE
- ESP8266 board

## Configuration

ESP8266 does not support `std::getenv()`, so configure settings at compile time.

### Option 1: PlatformIO
Edit `src/main.cpp`:
```cpp
const char *ssid = "your_wifi_ssid";
const char *password = "your_wifi_password";
const char *lightServerUrl = "http://your-vps-ip:3000/ldr-data";
const char *temperatureServerUrl = "http://your-vps-ip:3000/temperature";
const char *humidityServerUrl = "http://your-vps-ip:3000/humidity";
```

### Option 2: Arduino IDE

Copy the script into a new sketch, update the configuration lines as above, and upload.

### Troubleshooting
- Install the CH341SER driver if you have upload problems.
- Verify the NodeMCU appears correctly in Device Manager (under "Ports").
