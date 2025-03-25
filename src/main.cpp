#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>

const char *ssid = "your_wifi_ssid";
const char *password = "your_wifi_password";

const char *lightServerUrl = "http://your-vps-ip:3000/ldr-data";
const char *temperatureServerUrl = "http://your-vps-ip:3000/temperature";
const char *humidityServerUrl = "http://your-vps-ip:3000/humidity";

const int LDR_PIN = A0;
const int ONE_WIRE_BUS = D2;
const int DHT_PIN = D1;
#define DHTTYPE DHT11

const int LED_PIN = D4;
const int SWITCH_PIN = D3;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DHT dht(DHT_PIN, DHTTYPE);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, LED_PIN, NEO_GRB + NEO_KHZ800);

int currentMode = 0;
unsigned long lastSwitchPressTime = 0;
const unsigned long debounceDelay = 200;

void displayHumidity(float humidity);
void displayTemperature(float temperature);
void displayLDR(int ldrValue);
void sendDataToServer(int ldrValue, float temperatureC, float humidity);

void setup()
{
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  sensors.begin();
  dht.begin();
  strip.begin();
  strip.show();

  pinMode(SWITCH_PIN, INPUT_PULLUP);
}

void loop()
{
  int ldrValue = analogRead(LDR_PIN);
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float humidity = dht.readHumidity();

  if (digitalRead(SWITCH_PIN) == LOW)
  {
    unsigned long currentMillis = millis();

    if (currentMillis - lastSwitchPressTime > debounceDelay)
    {
      lastSwitchPressTime = currentMillis;
      currentMode = (currentMode + 1) % 3;
      Serial.print("Mode changed to: ");
      Serial.println(currentMode);
    }
  }

  switch (currentMode)
  {
  case 0:
    displayHumidity(humidity);
    break;
  case 1:
    displayTemperature(temperatureC);
    break;
  case 2:
    displayLDR(ldrValue);
    break;
  }

  delay(500);

  sendDataToServer(ldrValue, temperatureC, humidity);
}

void sendDataToServer(int ldrValue, float temperatureC, float humidity)
{
  HTTPClient http;
  WiFiClient client;

  String lightPostData = "{\"ldrValue\": " + String(ldrValue) + "}";
  String temperaturePostData = "{\"temperature\": " + String(temperatureC, 2) + "}";
  String humidityPostData = "{\"humidity\": " + String(humidity, 2) + "}";

  // Send Light Data
  http.begin(client, lightServerUrl);
  http.addHeader("Content-Type", "application/json");
  http.POST(lightPostData);
  http.end();

  // Send Temperature Data
  http.begin(client, temperatureServerUrl);
  http.addHeader("Content-Type", "application/json");
  http.POST(temperaturePostData);
  http.end();

  // Send Humidity Data
  http.begin(client, humidityServerUrl);
  http.addHeader("Content-Type", "application/json");
  http.POST(humidityPostData);
  http.end();
}

void displayHumidity(float humidity)
{
  int level = map(humidity, 0, 100, 0, 8);
  level = constrain(level, 0, 8);

  for (int i = 0; i < 8; i++)
  {
    if (i < level)
    {
      strip.setPixelColor(i, 0, 255, 0);
    }
    else
    {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  strip.show();
}

void displayTemperature(float temperature)
{
  int level = map(temperature, -10, 40, 0, 8);
  level = constrain(level, 0, 8);

  for (int i = 0; i < 8; i++)
  {
    if (i < level)
    {
      strip.setPixelColor(i, 0, 0, 255);
    }
    else
    {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  strip.show();
}

void displayLDR(int ldrValue)
{
  int level = map(ldrValue, 0, 1023, 0, 8);
  level = constrain(level, 0, 8);

  for (int i = 0; i < 8; i++)
  {
    if (i < level)
    {
      strip.setPixelColor(i, 255, 0, 0);
    }
    else
    {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  strip.show();
}
