#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

char* serverPool = "";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void initNTP() {
  // Initialize a NTPClient to get time
  timeClient.begin();
  
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(7200);

  
  // If the serverPool is defined, set timeClient pool to serverPool
  if(serverPool != "") timeClient.setPoolServerName(serverPool);
}

String formatData(int value) {
    if (value < 10) {
        return "0" + String(value);
    } else {
        return String(value);
    }
}

String getDateTime() {
  timeClient.forceUpdate();
  time_t t = timeClient.getEpochTime();
  return formatData(year(t))+"-"+formatData(month(t))+"-"+formatData(day(t))+"T"+formatData(hour(t))+":"+formatData(minute(t))+":"+formatData(second(t))+"Z";
}