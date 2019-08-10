
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
#include <DHT.h>

DHT dht;
AsyncWebServer server(80);

const char* ssid = "ap_301"; // Your WiFi SSID
const char* password = "123321123"; // Your WiFi Password
int indicator = 2;


void buttonClicked(const char* id){
   Serial.println("Button Clicked - "+String(id));
   digitalWrite(indicator, HIGH);
   delay(100);
   digitalWrite(indicator, LOW);
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
    
    dht.setup(14);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    ESPDash.init(server);   // Initiate ESPDash and attach your Async webserver instance
    // Add Respective Cards
    ESPDash.addNumberCard("num1", "Luz em volts", 264);
    ESPDash.addTemperatureCard("temp1", "Temperatura", 0, 20);
    ESPDash.addHumidityCard("hum1", "Umidade", 98);
    ESPDash.addButtonCard("btn1", "Blink Button");
    ESPDash.attachButtonClick(buttonClicked);
    server.begin();
}

void loop() {

    delay(5000);
    float sensorValue = analogRead(A0);
    float voltage = sensorValue * (5.0 / 1023.0);
    ESPDash.updateNumberCard("num1", voltage );

}