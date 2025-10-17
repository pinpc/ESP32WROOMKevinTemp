#include <dummy.h>

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string>
#include "weather.h"
#include <EEPROM.h>

const char* ssid = "xxxxxx";
const char* password = "xxxxxx";
const int8_t tempoffset = -5;
int restartcounter;
uint32_t heapfree = 0;
float heapusage;


/*const int analogPin = 34;

float error = 7.0;

std::string temp_out = "";
std::string temp_in = "";

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
    delay(2000);
    Serial.println("Connecting to WiFi...");
  }

  if(WiFi.status() != WL_CONNECTED){
      Serial.println("Failed to connect.");
      delay(3000);
      ESP.restart();
  } else {
      Serial.println("Connected!");
  }

  DisplayInit();

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
  char buff[7];
  float t,h;
  unsigned long currentMillis = millis();
  unsigned long previousMillis;
  const unsigned long interval = 5000;  // 5 seconds

  while(1){
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval){
      previousMillis = currentMillis;
      Serial.println("5 seconds passed");
      
      getWhetherTemp();
      t = sht31.readTemperature() + tempoffset;
      h = sht31.readHumidity(); 
      snprintf(buff, sizeof(buff), "%.2f", t);
      temp_in = std::string(buff);
      Serial.println(temp_out.c_str());
      Serial.println(temp_in.c_str());
      displayTemperature(temp_out, temp_in);

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
    }  
        
    displayTime();
    delay(1000);
      
  }
  
}
