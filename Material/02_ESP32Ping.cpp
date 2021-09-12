#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>

String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";
String googlDotCom = "www.google.com";


void setup()
{
    Serial.begin(9600);
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

    if (Ping.ping(googlDotCom.c_str()))
    {
        Serial.println("Connected to Router");
    }
}

void loop()
{
   
}
