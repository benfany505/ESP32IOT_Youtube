#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

// Deklarasi Variable dan Konstanta
String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";
const IPAddress routerIp(192, 168, 0, 1);
String googlDotCom = "www.google.com";

int randTemp, randHum;
AsyncWebServer server(80);

// Deklarasi Fungsi / Mthod
void connectWifi();
void getHttp();

void postHttp();
String readDHTTemperature();
String readDHTHumidity();
String processor(const String &var);

const char realtime_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link
      href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css"
      rel="stylesheet"
      integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC"
      crossorigin="anonymous"
    />

    <title>NodeMCU ESP32 IOT</title>
  </head>
  <body>
    <div class="container text-center"><h1>NodeMCU ESP32 IOT</h1></div>
    <div class="container bg-dark mt-3">
      <div class="d-flex text-center">
        <div class="container p-2">
          <h5 style="color: coral">Temperature</h5>
          <div class="card m-3">
            <h4 class="card-body" id="temp">%TEMPERATURE%</h4>
          </div>
        </div>

        <div class="container p-2">
          <h5 style="color: coral">Humidity</h5>
          <div class="card m-3">
            <h4 class="card-body" id="hum">%HUMIDITY%</h4>
          </div>
        </div>
      </div>
    </div>

    <script
      src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js"
      integrity="sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM"
      crossorigin="anonymous"
    ></script>

    <script>
      setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("temp").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "/temperature", true);
        xhttp.send();
      }, 1000 ) ;

      setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("hum").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "/humidity", true);
        xhttp.send();
      }, 1000 ) ;
      </script>

    
  </body>
</html>
)rawliteral";

void setup()
{
  Serial.begin(9600);
  connectWifi();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              request->send_P(200, "text/html", realtime_html, processor);
              Serial.println("Home");
            });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDHTTemperature().c_str()); });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDHTHumidity().c_str()); });

  // Start server
  server.begin();

  // getHttp();
}

void loop()
{
  Serial.println("Main");
  delay(1000);
}
String readDHTTemperature()
{
  float randDec = random(0, 99);
  randDec /= 100;
  int randTemp = random(20, 35);
  return String(randTemp + randDec);
}

String readDHTHumidity()
{
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float randDec = random(0, 99);
  randDec /= 100;
  int randHum = random(40, 80);
  return String(randHum + randDec);
}
// Replaces placeholder with DHT values
String processor(const String &var)
{
  //Serial.println(var);
  if (var == "TEMPERATURE")
  {
    return readDHTTemperature();
  }
  else if (var == "HUMIDITY")
  {
    return readDHTHumidity();
  }
  return String();
}
void postHttp()
{
  Serial.println("Posting...");
  String url = "http://192.168.0.93/esp32/api/api.php?data=insert";
  HTTPClient http;
  String response;

  StaticJsonDocument<200> buff;
  String jsonParams;

  randTemp = random(30, 50);
  randHum = random(40, 80);

  buff["temp"] = String(randTemp);
  buff["hum"] = String(randHum);

  serializeJson(buff, jsonParams);
  Serial.println(jsonParams);

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int statusCode = http.POST(jsonParams);
  response = http.getString();
  if (statusCode == 200)
  {
    Serial.println("Post Method Success!");
    Serial.println("Temp : " + randTemp);
    Serial.println("Hum : " + randHum);
  }
  else
  {
    Serial.println("Post Method Failed!");
  }
  // Serial.println(response);
  // Serial.println(statusCode);
}

void getHttp()
{
  String url = "https://reqres.in/api/users/2";
  HTTPClient http;
  String response;

  http.begin(url);
  http.GET();

  response = http.getString();
  Serial.println(response);

  StaticJsonDocument<1024> doc;
  deserializeJson(doc, response);
  JsonObject obj = doc.as<JsonObject>();

  String data = obj[String("data")];
  String support = obj[String("support")];

  Serial.println(data);
  Serial.println(support);
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

  if (Ping.ping(googlDotCom.c_str()))
  {
    Serial.println("Connected to Internet");
  }
}
