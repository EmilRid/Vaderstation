#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

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
}

String getDateTime() {
  timeClient.forceUpdate();
  time_t t = timeClient.getEpochTime();
  return String(year(t))+"-"+String(month(t))+"-"+String(day(t))+"T"+String(hour(t))+":"+String(minute(t))+":"+String(second(t))+"Z";
}