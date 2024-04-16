//#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "dht.h"

#define DHTPIN 17
#define DHTTYPE 11

DHT dht(DHTPIN, DHTTYPE);

int temp = -1;
int humidity = -1;

void setupDHT(){
    dht.begin();
}

void readSensor(){
    temp = dht.readTemperature();
    humidity = dht.readHumidity();
}

int getTemp(){
    return(temp);
}

int getHumidity(){
    return(humidity);
}

int getPercievedTemp(){
    //heat index = percieved temp. set isFahrenheit = false for celcius
    return(dht.computeHeatIndex(temp, humidity, false));
}
