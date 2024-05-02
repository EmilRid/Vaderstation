#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "dht11.h"
#include <Adafruit_BMP085.h>
#include "anemometer.h"
#include "json.h"

Adafruit_BMP085 bmp;
String stationName = "Default_station";
int temp;
int humidity;
int percievedTemp;
int32_t pressure;
float altitude;
float bmpTemp;

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
  setupAnemometer(26);

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

void readSensors() {
  readSensor();
  temp = getTemp();
  humidity = getHumidity();
  percievedTemp = getPercievedTemp();
  pressure = bmp.readPressure();
  altitude = bmp.readAltitude(101502);
  bmpTemp = bmp.readTemperature();
}

void outputDebug() {
  Serial.println();
  char str[50];
  sprintf(str, "Temp: %d", temp);
  Serial.println(str);
  sprintf(str, "Humidity :%d", humidity);
  Serial.println(str);
  sprintf(str, "Percieved Temp: %d", percievedTemp);
  Serial.println(str);
  delay(1000);
  sprintf(str, "Altitude: %d", altitude);
  Serial.println(str);
  sprintf(str, "Preassure: %d", pressure);
  Serial.println(str);
  sprintf(str, "BMP180 Temp: %d", bmpTemp);
  Serial.println(str);
}

void loop() {
  String serverName = "http://localhost:8184/json";
  boolean debug = true;

  readSensors();
  if(debug) outputDebug;  
  delay(10000);

  char testData[200];
  sprintf(testData, "{\n\"date\":\"1970-01-01T00:00:00Z\",\n\"temp\":\"%d\",\n\"humidity\":\"%d\",\n\"percievedTemp\":\"%d\",\n\"preassure\":\"%d\",\n\"altitude\":\"%f\"\n}", temp, humidity, percievedTemp, pressure, altitude);
  if(WiFi.status() == WL_CONNECTED){
    sendJson(serverName, testData.returnJson().c_str());
  }
}
