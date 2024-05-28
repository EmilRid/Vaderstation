//Author Emil
//Edited by Robin
#ifndef NTP_H
#define NTP_H
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include "config.h"

void initNTP();
String getDateTime();
#endif