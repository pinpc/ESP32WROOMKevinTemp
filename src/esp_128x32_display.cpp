#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
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


const char* ntpServer = "pool.ntp.org";
const long utcOffsetInSeconds = 3600;    //GMT+1 für DE
const int daylightOffset_sec = 3600;//+1 für Sommerzeit

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void clearDisplayArea(int x, int y, int w, int h) {
  u8g2.setDrawColor(0); // Set color to "background"
  u8g2.drawBox(x, y, w, h); // Clear the specified area
  u8g2.setDrawColor(1); // Reset color to "foreground"
}

void displayTemperature(){
  u8g2.begin();
  u8g2.enableUTF8Print();
  std::string temp_out2 = temp_out;
  //const char* temp_out3 = temp_out2.c_str(); 
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(0,60,temp_out2.c_str());
  Serial.println(temp_out2.c_str());
}

/*void displayTemp_in(){
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(60,60,temp_in.c_str());
}*/

void displayTemp_in2(){
  clearDisplayArea(64,32,64,32);
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(60,60,temp_in3.c_str());
}


void displayTime(){
  u8g2.setFont(u8g2_font_logisoso18_tf);
  clearDisplayArea(0,16,128,31);
  //clearDisplayArea(0,16,128,41);
  timeClient.update();
  String time = timeClient.getFormattedTime();
  
  u8g2.drawStr(5,40, time.c_str());
 //u8g2.drawStr(0,20, "21:50 Uhr");
  u8g2.sendBuffer(); 
  Serial.println(time);
}

void displaySystem( String restart, String heapuse){
  u8g2.setFont(u8g2_font_ncenB12_tr);
  clearDisplayArea(0,u8g2.getAscent(),u8g2.getDisplayWidth(),u8g2.getAscent());
  u8g2.drawStr(0,u8g2.getAscent(), restart.c_str());
  u8g2.drawStr(50,u8g2.getAscent(), heapuse.c_str());
  u8g2.sendBuffer(); 
  Serial.print(restart+"- ");
  Serial.println(heapuse);
}
