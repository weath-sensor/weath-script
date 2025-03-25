# ESP8266 Sensor Project

This project is an ESP8266 application that collects sensor data and sends it to configurable server endpoints. The project is configured for use with PlatformIO in VSCode, but an alternative configuration for the Arduino IDE is also provided.

## Features

- Collects sensor data (light, temperature, humidity)
- Sends data to server endpoints via HTTP POST requests
- Uses libraries such as ESP8266WiFi, ESP8266HTTPClient, OneWire, DallasTemperature, DHT, and Adafruit_NeoPixel

## Prerequisites

- **PlatformIO Extension in VSCode:** If you choose to use PlatformIO, ensure you have it installed in VSCode.
- **Arduino IDE:** Alternatively, you can copy the script and run it in the Arduino IDE.
- **ESP8266 Board:** This project is built for an ESP8266-based board.

## Configuration

Since the ESP8266 does not support reading environment variables via `std::getenv()`, you must configure the device settings at compile time or through a file-based configuration.

### Option 1: Compile-Time Configuration (PlatformIO)

1. Open the project in VSCode with the PlatformIO extension.
2. Edit the following lines in `main.cpp` (or your C++ file) to include your WiFi and server details:

   ```cpp
   const char *ssid = "your_wifi_ssid";
   const char *password = "your_wifi_password";
   const char *lightServerUrl = "http://your-vps-ip:3000/ldr-data";
   const char *temperatureServerUrl = "http://your-vps-ip:3000/temperature";
   const char *humidityServerUrl = "http://your-vps-ip:3000/humidity";
