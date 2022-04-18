#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32httpUpdate.h>

#define FIRMWARE_VERSION 1.2
#define UPDATE_URL "https://benfany505.github.io/firmware.json"

#define LED_PIN 13

TaskHandle_t task0;

// Deklarasi Variable dan Konstanta
String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";

// Deklarasi Fungsi
void connectWifi();
void checkUpdate();
void ledBlink(void *pvParameter);
t_httpUpdate_return updateOverHttp(String url_update);
void setup()
{
    Serial.begin(9600);
    connectWifi();
    pinMode(LED_PIN, OUTPUT);

    // create task core 0
    xTaskCreatePinnedToCore(ledBlink, "ledBlink", 10000, NULL, 1, &task0, 0);
}

void loop()
{
    checkUpdate();
    vTaskDelay(5000);
}

void ledBlink(void *pvParameter)
{
    for (;;)
    {
        // Your Code
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
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

void checkUpdate()
{
    Serial.println("Checking update");
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
        Serial.println(url_update);
        if (updateOverHttp(url_update) == HTTP_UPDATE_OK)
        {
            Serial.println("Update Success");
        }
        else
        {
            Serial.println("Update Failed");
        }
        Serial.println("Update Success");
    }
    else
    {
        Serial.println("No Update Available");
    }
}

t_httpUpdate_return updateOverHttp(String url_update)
{
    t_httpUpdate_return ret;

    if ((WiFi.status() == WL_CONNECTED))
    {

        ret = ESPhttpUpdate.update(url_update);

        switch (ret)
        {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            return ret;
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            return ret;
            break;

        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            return ret;
            break;
        }
    }
}