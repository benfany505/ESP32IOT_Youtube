#include <Arduino.h>
#include <WiFi.h>

// Deklarasi Variable dan Konstanta
String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";

// Deklarasi Fungsi
void connectWifi();

void setup()
{
  Serial.begin(9600);
  connectWifi();
}

void loop()
{

  Serial.println("test");
  delay(1000);
}

void connectWifi()
{
  Serial.println("Connecting To Wifi");
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Wifi Connected");
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.RSSI());
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.dnsIP());
}