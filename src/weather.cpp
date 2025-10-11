#include <WiFi.h>
#include <HTTPClient.h>

HTTPClient http;

String getWeatherRequest(){
  String textmessage;
  
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Internetverbinung nicht möglich");
  }
  else{
    http.begin("https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&current=temperature_2m");
    int readRequest = http.GET();
    if((readRequest > 0) && ( http.getSize() < 1024)){
      textmessage = http.getString();
      Serial.println(textmessage);
    }
    else{
      Serial.println("no Feedback");
    }
  }
  http.end();
  return(textmessage);
}