#include <Arduino.h>
#include <WiFi.h>
#include <CTBot.h>

// Deklarasi Variable dan Konstanta
String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";

TBMessage tMessage;
String token = "1975299142:AAHaGG9EX32RHcjGWuF5q_5luBQcway9oLo";
CTBot myBot;

// Deklarasi Fungsi
void connectWifi();
void loginTelegram();
String randTemp();
String randHum();
void setup()
{
    Serial.begin(9600);
    connectWifi();
    loginTelegram();
}

void loop()
{

    if (myBot.getNewMessage(tMessage))
    {

        Serial.println(tMessage.text);
        if (tMessage.messageType == CTBotMessageText)
        {
            if (tMessage.text.equalsIgnoreCase("/start"))
            {
                String reply = "";
                reply += "Hi, welcome to HobtechBot\n";
                reply += "Command List as below : \n";
                reply += "Get Temperature : /get_temp \n";
                reply += "Get Humidity : /get_hum \n";

                myBot.sendMessage(tMessage.sender.id, reply);
            }
            else if (tMessage.text.equalsIgnoreCase("/get_temp"))
            {
                String reply = "";
                reply += "Temperature Reading : \n";
                reply += randTemp() + " degC";
                myBot.sendMessage(tMessage.sender.id, reply);
            }
            else if (tMessage.text.equalsIgnoreCase("/get_hum"))
            {
                String reply = "";
                reply += "Humidity Reading : \n";
                reply = randHum() + " %";
                myBot.sendMessage(tMessage.sender.id, reply);
            }
            else
            {
                String reply = "";
                reply += "Hi, welcome to HobtechBot\n";
                reply += "Command is not valid plese use /start to get Command list! \n";

                myBot.sendMessage(tMessage.sender.id, reply);
            }
        }
    }
}

String randTemp()
{
    float randDec = random(0, 99);
    randDec /= 100;
    int randVal = random(20, 35);
    return String(randVal + randDec);
}

String randHum()
{
    float randDec = random(0, 99);
    randDec /= 100;
    int randVal = random(40, 80);
    return String(randVal + randDec);
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
    Serial.println("logging in...");
    while (!myBot.testConnection())
    {
        myBot.setTelegramToken(token);
        delay(1000);
    }
    Serial.println("Telegram connection OK!");
}