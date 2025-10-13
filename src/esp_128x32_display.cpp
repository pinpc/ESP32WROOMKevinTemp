#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <string>
#include "weather.h"


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

u8g2_uint_t offset;
u8g2_uint_t width;
std::string temp_out = "";
std::string temp_in = "";
Adafruit_SHT31 sht31;
const char* ntpServer = "pool.ntp.org";
const long utcOffsetInSeconds = 3600;    //GMT+1 für DE
const int daylightOffset_sec = 3600;//+1 für Sommerzeit

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void DisplayInit(){
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.clearBuffer();
}

void clearDisplayArea(int x, int y, int w, int h) {
  u8g2.setDrawColor(0); // Set color to "background"
  u8g2.drawBox(x, y, w, h); // Clear the specified area
  u8g2.setDrawColor(1); // Reset color to "foreground"
}

void displaySystem( String restart, String heapuse){
  u8g2.setFont(u8g2_font_ncenB08_tr);
  clearDisplayArea(0, 0, 128, 12);
  u8g2.drawStr(0, 12, restart.c_str());
  u8g2.drawStr(90, 12, heapuse.c_str());
  u8g2.sendBuffer(); 
  Serial.print(restart+"- ");
  Serial.println(heapuse);
}

void displayTime(){
  u8g2.setFont(u8g2_font_ncenB14_tr);
  clearDisplayArea(0, 12, 127, 21);
  timeClient.update();
  String time = timeClient.getFormattedTime();
  
  u8g2.drawStr(15, 40, time.c_str()); //"21:50:12" 
  u8g2.sendBuffer(); 
  Serial.println(time);
}

void displayTemperature(std::string temp_out, std::string temp_in){
   
  u8g2.setFont(u8g2_font_ncenB12_tr);
  clearDisplayArea(0, 41, 127, 23);
  u8g2.drawStr(0,64,temp_out.c_str());
  u8g2.drawStr(60,64,temp_in.c_str());
  u8g2.sendBuffer(); 
  Serial.println(temp_out.c_str());
  Serial.println(temp_in.c_str());
}

void getWhetherTemp(){
  float value;
  char buff[7];
  
  const char* forecast = getWeatherRequest().c_str();
  StaticJsonDocument<1024> doc;
  // JSON-String in ein JSON-Objekt parsen
  DeserializationError error = deserializeJson(doc, forecast);
  
  if (error) {
    Serial.print("JSON-Parsing fehlgeschlagen: ");
    Serial.println(error.c_str());
    return;
  }
  else{
    value = doc["current"]["temperature_2m"];
    snprintf(buff, sizeof(buff), "%.2f", value);
    temp_out = std::string(buff);
    Serial.println(temp_out.c_str());
  }  
}
