// #include <Arduino.h>
// #include <WiFi.h>
// #include <PubSubClient.h>
// #include <ThingsBoard.h>

// // Deklarasi Variable dan Konstanta
// String wifiSSID = "smartbuilding_wifi";
// String wifiPassword = "smartbuilding@2020";

// String thingsHost = "thingsboard.cloud";
// String thingsToken = "g8GNTHWCfcYRd6KaIjkg";
// unsigned long lastSend;

// WiFiClient client;

// ThingsBoard tb(client);

// // Deklarasi Fungsi

// void connectWifi()
// {
//     Serial.println("Connecting To Wifi");
//     WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
//     while (WiFi.status() != WL_CONNECTED)
//     {
//         Serial.print(".");
//         delay(500);
//     }

//     Serial.println("Wifi Connected");
//     Serial.println(WiFi.SSID());
//     Serial.println(WiFi.RSSI());
//     Serial.println(WiFi.macAddress());
//     Serial.println(WiFi.localIP());
//     Serial.println(WiFi.gatewayIP());
//     Serial.println(WiFi.dnsIP());
// }

// String randTemp()
// {
//     float randDec = random(0, 99);
//     randDec /= 100;
//     int randVal = random(25, 35);
//     return String(randVal + randDec);
// }

// String randHum()
// {
//     float randDec = random(0, 99);
//     randDec /= 100;
//     int randVal = random(40, 60);
//     return String(randVal + randDec);
// }

// void getAndSendTemperatureAndHumidityData()
// {

//     float hum = randHum().toDouble();
//     float temp = randTemp().toDouble();

//     Serial.println("Sending data to ThingsBoard:");
//     Serial.print("Humidity: ");
//     Serial.print(hum);
//     Serial.print(" %\t");
//     Serial.print("Temperature: ");
//     Serial.print(temp);
//     Serial.println(" *C ");

//     tb.sendTelemetryFloat("temperature", temp);
//     tb.sendTelemetryFloat("humidity", hum);
// }

// void reconnect()
// {
//     // Loop until we're reconnected
//     while (!tb.connected())
//     {

//         if (WiFi.status() != WL_CONNECTED)
//         {
//             connectWifi();
//         }
//         Serial.print("Connecting to ThingsBoard node ...");
//         if (tb.connect(thingsHost.c_str(), thingsToken.c_str()))
//         {
//             Serial.println("[DONE]");
//         }
//         else
//         {
//             Serial.print("[FAILED]");
//             Serial.println(" : retrying in 5 seconds]");
//             // Wait 5 seconds before retrying
//             delay(5000);
//         }
//     }
// }
// void setup()
// {
//     Serial.begin(9600);
//     connectWifi();
// }

// void loop()
// {

//     if (!tb.connected())
//     {
//         reconnect();
//     }

//     if (millis() - lastSend > 1000)
//     { // Update and send only after 1 seconds
//         getAndSendTemperatureAndHumidityData();
//         lastSend = millis();
//     }

//     tb.loop();
// }