
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
#include <DHT.h>

AsyncWebServer server(80);

const char* ssid = "ap_301"; // Your WiFi SSID
const char* password = "123321123"; // Your WiFi Password
int indicator = 2;
#define DHTPIN 2
#define DHTTYPE    DHT11
#define rele D1

DHT dht(DHTPIN, DHTTYPE);

String releOutputState = "off";
boolean releStatus = false;

void buttonClicked(const char* id){
   Serial.println("Button Clicked - "+String(id));

   if(releOutputState.length() <= 2){
       releOutputState = "off";
        digitalWrite(rele, LOW);
        releStatus = false;

   }
   else{
       releOutputState = "on";
        digitalWrite(rele, HIGH);
        releStatus = true;

   }


}

void setup() {
    pinMode(rele, OUTPUT);
    digitalWrite(rele, LOW);

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
    dht.begin();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    ESPDash.init(server);   // Initiate ESPDash and attach your Async webserver instance
    // Add Respective Cards
    ESPDash.addNumberCard("num1", "Luz em volts", 0);
    ESPDash.addTemperatureCard("temp1", "Temperatura", 0, 20);
    ESPDash.addHumidityCard("hum1", "Umidade", 98);
    ESPDash.addButtonCard("rele_1", "Rele");   
    ESPDash.addStatusCard("rele_1","Rele Status");
    ESPDash.attachButtonClick(buttonClicked);
    

    server.begin();
}

void loop() {

      delay(10000);
ESPDash.updateStatusCard("rele_1",releStatus);
ESPDash.updateNumberCard("num1", (analogRead(A0)*(5.0 / 1023.0)));
String t = dht.readTemperature();
String h = dht.readHumidity();
ESPDash.updateHumidityCard("hum1", h);
ESPDash.updateTemperatureCard("temp1", t);
float temperature;
float bTemperature;

temperature = dht.readTemperature();

if(!isnan(temperature)){
  bTemperature = temperature;
  Serial.println(temperature);

}
else{
  Serial.println(bTemperature);
}
Serial.println(t, h);

}