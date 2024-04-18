#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "dht11.h"
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

void initWiFi(String ssid, String password) {
  // Enable station mode (wifi client) and disconnect from previous
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  //25s to connect
  for(int i=0; i<2; i++){
    if(WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
    else{
      Serial.println(WiFi.localIP());
      i = 1000; //break 
    }
  }
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Not connected!");
  }
  
}

void sendJson(String serverPath, String jsonData){
  HTTPClient http;
  
  http.begin(serverPath.c_str());
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonData);
  Serial.print(httpResponseCode);
}

void setup() {
  Serial.begin(9600);
  setupDHT();

  if (!bmp.begin()) {
	  Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");
	}
  else{
    Serial.println("BMP found!");
  }

  Serial.println("Setup done");
  //enter wifi details.
  initWiFi("Hugos iPhone", "hugowifi12333");
}

void loop() {
  String serverName = "http://localhost:8184/json";
  
  readSensor();
  Serial.println();
  char str[50];
  sprintf(str, "Temp: %d", getTemp());
  Serial.println(str);
  sprintf(str, "Humidity :%d", getHumidity());
  Serial.println(str);
  sprintf(str, "Percieved Temp: %d", getPercievedTemp());
  Serial.println(str);
  delay(1000);
  char testData[200];
  //this is bad. make new file for bmp reading that handles this instead
  if(bmp.begin()){
    int tempValue = bmp.readAltitude(101502);
    sprintf(str, "Altitude: %d", tempValue);
    Serial.println(str);
    tempValue = bmp.readPressure();
    sprintf(str, "Preassure: %d", tempValue);
    Serial.println(str);
    tempValue = bmp.readTemperature();
    sprintf(str, "BMP180 Temp: %d", tempValue);
    Serial.println(str);
    sprintf(testData, "{\n\"date\":\"1970-01-01T00:00:00Z\",\n\"temp\":\"%d\",\n\"humidity\":\"%d\",\n\"percievedTemp\":\"%d\",\n\"preassure\":\"%d\",\n\"altitude\":\"%f\"\n}", getTemp(), getHumidity(), getPercievedTemp(), bmp.readPressure(), bmp.readAltitude(101502));
  }
  else{
    sprintf(testData, "{\n\"date\":\"1970-01-01T00:00:00Z\",\n\"temp\":\"%d\",\n\"humidity\":\"%d\",\n\"percievedTemp\":\"%d\",\n\"preassure\":\"%d\",\n\"altitude\":\"%f\"\n}", getTemp(), getHumidity(), getPercievedTemp(), 0, 0);
  }

  if(WiFi.status() == WL_CONNECTED){
    sendJson(serverName, testData);
  }
  delay(10000);
}
