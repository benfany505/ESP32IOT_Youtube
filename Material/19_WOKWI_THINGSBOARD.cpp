#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ThingsBoard.h>

// Deklarasi Variable dan Konstanta
String wifiSSID = "Wokwi-GUEST";
String wifiPassword = "";

// Thingsboard Credential
String tbHost = "demo.thingsboard.io";
String tbToken = "C9omLqpnmP3BRD5PRbdO";

WiFiClient client;
ThingsBoard tb(client);

// Deklarasi Fungsi
void connectWifi();
String randTemp();
String randHum();
void tbReconnect();

void setup()
{
    Serial.begin(9600);
    connectWifi();
}

void loop()
{
    if (!tb.connected())
    {
        tbReconnect();
    }

    float temp = randTemp().toDouble();
    float hum = randHum().toDouble();

    Serial.println("Sending data to Thingsboard");
    tb.sendTelemetryFloat("temperature", temp);
    tb.sendTelemetryFloat("humidity", hum);
    tb.loop();
    delay(1000);
}

String randTemp()
{
    float randDec = random(0, 99);
    randDec /= 100;
    int randVal = random(25, 30);
    return String(randVal + randDec);
}

String randHum()
{
    float randDec = random(0, 99);
    randDec /= 100;
    int randVal = random(40, 60);
    return String(randVal + randDec);
}

void tbReconnect()
{
    while (!tb.connected())
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            connectWifi();
        }

        Serial.println("connecting to thingsboard ... ");
        if (tb.connect(tbHost.c_str(), tbToken.c_str()))
        {
            Serial.println("Thingsboard Connected!");
        }
        else
        {
            Serial.println("Thingsboard connection failed");
            Serial.println("Retrying in 5 seconds...");
            delay(5000);
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