#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32httpUpdate.h>

#define FIRMWARE_VERSION 1.2
#define UPDATE_URL "https://benfany505.github.io/firmware.json"

#define LED_PIN 12
#define LED_2 13

// Deklarasi Variable dan Konstanta
String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";

TaskHandle_t task0;

// Deklarasi Fungsi
void connectWifi();
void checkUpdate();
void ledBlink(void *pvParameters);
t_httpUpdate_return updateFirmware(String url_update);

void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(LED_2, OUTPUT);
    connectWifi();
    xTaskCreatePinnedToCore(
        ledBlink,
        "checkUpdate",
        10000,
        NULL,
        1,
        &task0,
        0);
}

void loop()
{

    checkUpdate();
    delay(5000);
}

void ledBlink(void *pvParameters)
{

    while (1)
    {
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(LED_2, LOW);
        delay(500);
        digitalWrite(LED_2, HIGH);
        digitalWrite(LED_PIN, LOW);
        delay(500);
    }
}

void checkUpdate()
{

    Serial.println("Checking Update");
    HTTPClient http;
    String response;
    String url = UPDATE_URL;
    http.begin(url);
    http.GET();

    response = http.getString();

    Serial.println(response);

    StaticJsonDocument<1024> doc;
    deserializeJson(doc, response);
    JsonObject obj = doc.as<JsonObject>();

    String version = obj[String("version")];
    String url_update = obj[String("url")];

    Serial.println(version);
    Serial.println(url_update);

    if (version.toDouble() > FIRMWARE_VERSION)
    {
        Serial.println("Update Available");
        if (updateFirmware(url_update) == HTTP_UPDATE_OK)
        {
            Serial.println("Update Success");
            ESP.restart();
        }
        else
        {
            Serial.println("Update Failed");
        }
    }
    else
    {
        Serial.println("No Update Available");
    }
}

t_httpUpdate_return updateFirmware(String url_update)
{
    t_httpUpdate_return ret;

    if ((WiFi.status() == WL_CONNECTED))
    {

        ret = ESPhttpUpdate.update(url_update);

        switch (ret)
        {
        case HTTP_UPDATE_FAILED:
            Serial.println("[update] Update failed.");
            return ret;
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("[update] Update no Update.");
            return ret;
            break;

        case HTTP_UPDATE_OK:
            Serial.println("[update] Update ok.");
            return ret;
            break;
        }
    }
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