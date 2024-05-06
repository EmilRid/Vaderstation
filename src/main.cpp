#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "dht11.h"
#include <Adafruit_BMP085.h>
#include "anemometer.h"
#include "json.h"
#include "ntp.h"

Adafruit_BMP085 bmp;
String stationName = "Default_station";
int temp;
int humidity;
int percievedTemp;
int32_t pressure;
float altitude;
float bmpTemp;
String dateTime;

void initWiFi(String ssid, String password) {
  // Enable station mode (wifi client) and disconnect from previous
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  //25s to connect
  for(int i=0; i<10; i++){
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
  
  http.begin((serverPath + "/json").c_str());
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonData);
  Serial.print(httpResponseCode);
}

bool checkCondition(String serverPath){
  HTTPClient http;

  http.begin((serverPath + "/condition").c_str());
  int httpResponseCode = http.GET();
  return httpResponseCode == 200;
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
  initNTP();
  if(WiFi.status() == WL_CONNECTED){
    dateTime = getDateTime();
  }
}

void readSensors() {
  dhtReadSensor();
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

void makeJson(Json& jsonData){
  jsonData.addValue("Date", dateTime.c_str());
  jsonData.addValue("Temperature", std::to_string(temp));
  jsonData.addValue("humidity", std::to_string(humidity));
  jsonData.addValue("percievedTemp", std::to_string(percievedTemp));
  jsonData.addValue("altitude", std::to_string(altitude));
  jsonData.addValue("pressure", std::to_string(pressure));
}

void loop() {
  String serverName = "http://hugoblomdahl.se:8184";
  boolean debug = true;

  readSensors();
  if(debug) outputDebug;  
  delay(10000);

  Json testData = Json();
  makeJson(testData);
  Serial.println(testData.returnJson().c_str());
  if(WiFi.status() == WL_CONNECTED){
    if(checkCondition(serverName)){
      sendJson(serverName, testData.returnJson().c_str());
    }
  }
}
