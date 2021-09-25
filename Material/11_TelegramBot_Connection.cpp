#include <Arduino.h>
#include <WiFi.h>
#include <CTBot.h>

// Deklarasi Variable dan Konstanta
String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";
CTBot myBot;
String token = "2014994585:AAG_4--yI3CrcTW1c-rbRPBimqlEVOCVWLc";

// Deklarasi Fungsi
void connectWifi();
void loginTelegram();
void setup()
{
    Serial.begin(9600);
    connectWifi();
    loginTelegram();
}

void loop()
{

    //Your Code
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

void loginTelegram()
{
    Serial.print("login telegram");

    while (!myBot.testConnection())
    {
        myBot.setTelegramToken(token);
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.println("Telegram Connection OK");
}