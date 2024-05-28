//Author Hugo
#include <Arduino.h>
#ifndef ANEMOMETER_H
#define ANEMOMETER_H

void setupAnemometer(int pinNum);
double readRPM(int seconds);

#endif