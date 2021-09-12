#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Deklarasi Variable dan Konstanta
String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";
String mqttBroker = "test.mosquitto.org";
WiFiClient client;
PubSubClient mqtt(client);

// Deklarasi Fungsi
void connectWifi();
void connect_mqtt();
void setup()
{
  Serial.begin(9600);
  connectWifi();
  mqtt.setServer(mqttBroker.c_str(), 1883);
}

void loop()
{
  if (!mqtt.connected())
  {
    connect_mqtt();
    Serial.println("MQTT Connected");
  }
  mqtt.loop();
}

void connect_mqtt()
{
  while (!mqtt.connected())
  {
    Serial.println("Connecting MQTT...");
    if (mqtt.connect("esp32"))
    {
      mqtt.subscribe("esp32/temphum");
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