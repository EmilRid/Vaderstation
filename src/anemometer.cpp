#include "anemometer.h"
int counter = 0;

void sensorTriggerd(){
    counter++;
    Serial.println("Magnet passed!");
    Serial.print(counter);
}

void setupAnemometer(int pinNum){
    pinMode(pinNum, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinNum), sensorTriggerd, FALLING);
}
