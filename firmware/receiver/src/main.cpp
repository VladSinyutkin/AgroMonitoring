#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>

// Настройки Wi-Fi
const char *ssid = "ВАШ_WIFI_SSID";
const char *password = "ВАШ_WIFI_PASSWORD";

// Настройки NRF24L01+
#define CE_PIN D2  // Пин CE (подключите к D2 на NodeMCU)
#define CSN_PIN D1 // Пин CSN (подключите к D1)
RF24 radio(CE_PIN, CSN_PIN);

// Адрес радиоканала (должен совпадать с передатчиком)
const uint64_t pipeAddress = 0xF0F0F0F0E1LL;

// Структура для приема данных
struct SensorData
{
    float temperature;
    float humidity;
};

void setup()
{
    Serial.begin(115200);

    // Подключение к Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    // Инициализация NRF24L01+
    radio.begin();
    radio.setChannel(5);
    radio.setDataRate(RF24_1MBPS);
    radio.setPALevel(RF24_PA_HIGH);
    radio.openReadingPipe(1, pipeAddress);
    radio.startListening(); // Режим приемника

    Serial.println("Receiver ready!");
}

void loop()
{
    if (radio.available())
    {
        SensorData data;
        radio.read(&data, sizeof(data));

        Serial.print("Получено: Temp = ");
        Serial.print(data.temperature);
        Serial.print("°C, Hum = ");
        Serial.print(data.humidity);
        Serial.println("%");

        // Здесь можно добавить отправку в облако (ThingSpeak, MQTT и т. д.)
        // Например:
        // sendToCloud(data.temperature, data.humidity);
    }
    delay(100);
}