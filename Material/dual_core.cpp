#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";
String googlDotCom = "www.google.com";
TaskHandle_t task_1;
TaskHandle_t task_2;
void getHttp();
void Task1code(void *pvParameters);
// void Task2code(void *pvParameters);
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
  Serial.println(xPortGetCoreID());
  xTaskCreatePinnedToCore(
      Task1code, /* Task function. */
      "Task1",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &task_1,   /* Task handle to keep track of created task */
      0);        /* pin task to core 0 */
  delay(500);

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  // xTaskCreatePinnedToCore(
  //     Task2code, /* Task function. */
  //     "Task2",   /* name of task. */
  //     10000,     /* Stack size of task */
  //     NULL,      /* parameter of the task */
  //     1,         /* priority of the task */
  //     &task_2,   /* Task handle to keep track of created task */
  //     1);        /* pin task to core 1 */
  // delay(500);
  //
}

void loop()
{

  Serial.println("Looping");
  delay(1000);

  // Serial.println(Ping.averageTime());

  // delay(1000);
  // put your main code here, to run repeatedly:
}

void getHttp()
{
  Serial.println("Get");
  HTTPClient http;
  String url = "https://reqres.in/api/users?delay=3";
  String response;

  StaticJsonDocument<1024> doc;
  StaticJsonDocument<1024> data;
  StaticJsonDocument<1024> support;

  http.begin(url);
  http.GET();
  response = http.getString();
  Serial.println(response);
  // deserializeJson(doc, response);
  // // JsonObject obj = doc.as<JsonObject>();
  // String datas = doc.as<JsonObject>()[String("data")];
  // deserializeJson(data, datas);
  // String email = data.as<JsonObject>()[String("email")];
  // Serial.println(email);
}

void Task1code(void *pvParameters)
{
  for (;;)
  {
    Serial.println("Get");
    HTTPClient http;
    String url = "https://reqres.in/api/users?delay=3";
    String response;

    StaticJsonDocument<1024> doc;
    StaticJsonDocument<1024> data;
    StaticJsonDocument<1024> support;

    http.begin(url);
    http.GET();
    response = http.getString();
    Serial.println("get Response");
    // vTaskDelete(NULL);
  }
}

//Task2code: blinks an LED every 700 ms
// void Task2code(void *pvParameters)
// {
//   Serial.print("Task2 running on core ");
//   Serial.println(xPortGetCoreID());

//   for (;;)
//   {
//     Serial.println("YYY");
//     delay(700);
//   }
// }