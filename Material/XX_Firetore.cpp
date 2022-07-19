#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>

/* 2. Define the API Key */
#define API_KEY "AIzaSyAsCQl6mgw6gwZWMR0nKKkgR4azFhInouQ"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "esp32test-cfa15"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "admin@hobtech.com"
#define USER_PASSWORD "hobtech1234"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

// Deklarasi Variable dan Konstanta
String wifiSSID = "smartbuilding_wifi";
String wifiPassword = "smartbuilding@2020";

// Deklarasi Fungsi
void connectWifi();
String randHum();
String randTemp();
void sendDataToFirebase();
void updateDataToFirebase();

void setup()
{
    Serial.begin(9600);
    connectWifi();
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);

    // setup core 1 task
    updateDataToFirebase();
}

void loop()
{

    // Your Code
}

// send data to firebase
void sendDataToFirebase()
{
    String documentPath = "Home/Room";
    FirebaseJson content;
    content.set("fields/temperature/doubleValue", randTemp().c_str());
    content.set("fields/humidity/doubleValue", randHum().c_str());

    if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
        Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    else
        Serial.println(fbdo.errorReason());
}

void updateDataToFirebase()
{
    String documentPath = "Home/Room";
    FirebaseJson content;
    content.set("fields/temperature/doubleValue", randTemp().c_str());
    content.set("fields/humidity/doubleValue", randHum().c_str());

    Serial.println(content.raw());

    if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw(), "temperature,humidity" /* updateMask */))
        Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    else
        Serial.println(fbdo.errorReason());
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
    int randVal = random(40, 70);
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
