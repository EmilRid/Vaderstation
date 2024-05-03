//#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 17
#define DHTTYPE 11

DHT dht(DHTPIN, DHTTYPE);

int dhtTemp = -1;
int dhtHumidity = -1;

void setupDHT(){
    dht.begin();
}
//changeme, bad name
void dhtReadSensor(){
    dhtTemp = dht.readTemperature();
    dhtHumidity = dht.readHumidity();
}

int getTemp(){
    return(dhtTemp);
}

int getHumidity(){
    return(dhtHumidity);
}

int getPercievedTemp(){
    //heat index = percieved temp. set isFahrenheit = false for celcius
    return(dht.computeHeatIndex(dhtTemp, dhtHumidity, false));
}
