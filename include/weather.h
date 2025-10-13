#ifndef WEATHER
#define WEATHER

#include <U8g2lib.h>
#include <Adafruit_SHT31.h>

extern Adafruit_SHT31 sht31;

extern std::string temp_out;
extern std::string temp_in;
extern float t;
extern float h;

void DisplayInit();
void displaySystem( String restart, String heapuse);
void displayTime();
void displayTemperature(std::string temp_out, std::string temp_in);
void getWhetherTemp();
String getWeatherRequest();
#endif