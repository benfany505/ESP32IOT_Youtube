#include <Arduino.h>
#include <WiFi.h>
#include <CTBot.h>

// Deklarasi Variable dan Konstanta
String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";
CTBot myBot;
String token = "2014994585:AAG_4--yI3CrcTW1c-rbRPBimqlEVOCVWLc";
TBMessage tMessage;
#define TEMP_CALLBACK "/get_temp"
#define HUM_CALLBACK "/get_hum"

CTBotInlineKeyboard myKbd;
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

    //Your Code

    if (myBot.getNewMessage(tMessage))
    {

        Serial.println(tMessage.sender.firstName);
        if (tMessage.messageType == CTBotMessageText)
        {
            Serial.println(tMessage.text);
            if (tMessage.text.equalsIgnoreCase("/start"))
            {
                String reply = "";
                reply += "Hi, welcome to HobTechTv\n";
                reply += "Press button below to send command\n";
                myBot.sendMessage(tMessage.sender.id, reply, myKbd);
            }
        }

        else if (tMessage.messageType == CTBotMessageQuery)
        {
            Serial.println(tMessage.callbackQueryData);
            if (tMessage.callbackQueryData.equals(TEMP_CALLBACK))
            {
                String reply = randTemp();
                myBot.endQuery(tMessage.callbackQueryID, reply + " degC", true);
                delay(1000);
            }
            else if (tMessage.callbackQueryData.equals(HUM_CALLBACK))
            {
                String reply = randHum();
                myBot.endQuery(tMessage.callbackQueryID, reply + " %", true);
                delay(1000);
            }
        }
    }
}

String randTemp()
{
    float randDec = random(0, 99);
    randDec /= 100;
    int randVal = random(20, 40);
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
    Serial.print("login telegram");

    while (!myBot.testConnection())
    {
        myBot.setTelegramToken(token);
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.println("Telegram Connection OK");
    myKbd.addButton("GET TEMP", TEMP_CALLBACK, CTBotKeyboardButtonQuery);
    myKbd.addButton("GET HUMIDITY", HUM_CALLBACK, CTBotKeyboardButtonQuery);
}