#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#ifndef NTP_H
#define NTP_H

void initNTP();
String getDateTime();

#endif