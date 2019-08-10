
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
#include <DHTesp.h>

AsyncWebServer server(80);

const char* ssid = "ap_301"; // Your WiFi SSID
const char* password = "123321123"; // Your WiFi Password
int indicator = 2;
#define DHTpin 2
#define DHTTYPE    DHT11

DHTesp dht;


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
    dht.setup(DHTpin, DHTesp::DHT11);

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
  delay(dht.getMinimumSamplingPeriod());

float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(temperature);
  Serial.print("\t\t");
  Serial.print(humidity);
  Serial.print("\t\t");
  
    float sensorValue = analogRead(A0);
    float voltage = sensorValue * (5.0 / 1023.0);
    ESPDash.updateNumberCard("num1", voltage );

}