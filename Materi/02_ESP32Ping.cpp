#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <HTTPClient.h>

String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";
const IPAddress routerIp(192, 168, 0, 1);
String googlDotCom = "www.google.com";

void getHttp();

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
    Serial.println(Ping.averageTime());
    delay(1000);
    // put your main code here, to run repeatedly:
}
