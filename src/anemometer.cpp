#include "anemometer.h"
int counter = 0;
int pinNum;

void sensorTriggerd(){
    counter++;
}

void setupAnemometer(int pin){
    pinMode(pin, INPUT_PULLUP);
    pinNum = pin;
}

double readRPM(int seconds){
    counter = 0;
    unsigned long startTime = millis();
    attachInterrupt(digitalPinToInterrupt(pinNum), sensorTriggerd, FALLING);
    delay(seconds * 1000);
    detachInterrupt(digitalPinToInterrupt(pinNum));
    return((counter / seconds) * 60.0);
}
