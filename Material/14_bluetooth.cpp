#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

void setup()
{
    Serial.begin(9600);
    SerialBT.begin("ESP32_Bluetooth");
    delay(3000);
    Serial.println("Bluetooth Ready");
}

void loop()
{
    // cek data serial BT yang masuk
    if (SerialBT.available())
    {
        String dataMasuk = SerialBT.readString();
        Serial.print(dataMasuk);
    }

    // cek data Serial dari Terminal
    if (Serial.available())
    {
        String dataKeluar = Serial.readString();
        SerialBT.print(dataKeluar);
    }
}