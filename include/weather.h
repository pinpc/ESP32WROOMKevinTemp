#ifndef WEATHER_H
#define WEATHER_H

#include <U8g2lib.h>
#include <Adafruit_SHT31.h>

extern Adafruit_SHT31 sht31;

extern std::string temp_out;
extern std::string temp_in2;
extern std::string temp_in3;
extern float t;
extern float h;
//extern std::string temp_in;
//extern std::string voltage_in;


void displayTemperature();
void displayTime();
void displayTemp_in2();
void displaySystem( String restart, String heapuse);
//void displayTemp_in();
String getWeatherRequest(); 
#endif