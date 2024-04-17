#include <Adafruit_Sensor.h>

#ifndef DHT11_SENSOR_H
#define DHT11_SENSOR_H
void setupDHT();
void readSensor();
int getTemp();
int getHumidity();
int getPercievedTemp();
#endif