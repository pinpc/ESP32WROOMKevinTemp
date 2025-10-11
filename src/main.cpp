#include <dummy.h>

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string>
#include "weather.h"
#include <EEPROM.h>

const char* ssid = "Fritz-ZW";
const char* password = "Fritzbox!2020";
int restartcounter;
uint32_t heapfree = 0;
float heapusage;


/*const int analogPin = 34;

float error = 7.0;

std::string voltage_in = "";
std::string temp_in = "";*/
std::string temp_out = "";
std::string temp_in3 = "";

Adafruit_SHT31 sht31 = Adafruit_SHT31();

/*float Thermistor(int adc) {
  float t = 0.0;
  float R2 = 10000.0 / ((4095.0 / adc) - 1.0);
  t = R2 / 10000.0;
  t = log(t) / 3950;
  t = t + 1,0 / (25,0 + 273,15);
  t = (1.0 / t) - 273.15; // In Celsius
  return t + error;
}*/

void EEPROMIni()
{
  EEPROM.begin(512);  // Allocate 512 bytes in flash

  restartcounter = EEPROM.read(0);
  restartcounter++;
  EEPROM.write(0, restartcounter);
  EEPROM.commit();

  Serial.print("Restart count: ");
  Serial.println(restartcounter);
}

void setup() {
  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 20000; // 10 seconds
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);
  
//  http.POST("ca.kawlan.com/?login=&username=&password=&submit=START&...=false")
  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  if(WiFi.status() != WL_CONNECTED){
      Serial.println("Failed to connect.");
      delay(1000);
      ESP.restart();
  } else {
      Serial.println("Connected!");
  }

  if (!sht31.begin(0x44)) {   // Standard I²C-Adresse
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  EEPROMIni();
  heapfree = ESP.getFreeHeap();
  
}

void loop() {
  const char* forecast = getWeatherRequest().c_str();
  char str1[4]="";
  char str2[5]="";

  StaticJsonDocument<1024> doc;

  // JSON-String in ein JSON-Objekt parsen
  DeserializationError error = deserializeJson(doc, forecast);

  if (error) {
    Serial.print("JSON-Parsing fehlgeschlagen: ");
    Serial.println(error.c_str());
    return;
  }
  float value = doc["current"]["temperature_2m"];
  char buff[7];
  snprintf(buff, sizeof(buff), "%.2f", value);
  temp_out = std::string(buff);
  Serial.println(temp_out.c_str());
  displayTemperature();
  int i = 0;
  while(i<=3600){
    if(i % 5 == 0){
      /*int adc = analogRead(analogPin);
      float voltage_in = 49 - (adc * 3.3 / 4095 *100);
      Serial.println(adc);
      char buff2[7];
      snprintf(buff2, sizeof(buff2), "%.2f", voltage_in);
      temp_in = std::string(buff2);
      Serial.println(temp_in.c_str());
      displayTemp_in();*/
      float t = sht31.readTemperature();
      float h = sht31.readHumidity(); 
      snprintf(buff, sizeof(buff), "%.2f", t);
      temp_in3 = std::string(buff);
      Serial.println(temp_in3.c_str());
      displayTemp_in2();
      Serial.println(ESP.getFreeHeap());
      heapusage = (float)ESP.getFreeHeap()/heapfree *100;
      if (heapusage <= 10){ // percent
          ESP.restart();
          Serial.println(heapusage);
      }
      
      restartcounter = EEPROM.read(0);
      sprintf(str1, "%d", restartcounter);
      sprintf(str2, "%.2f", heapusage);      
      displaySystem(str1, str2);
      Serial.print("Restart count: ");
      Serial.println(restartcounter);
      Serial.print("Heapusage: ");
      Serial.println(heapusage);
    }
    displayTime();
    delay(1000);
    i++;
  }
  
}
