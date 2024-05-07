#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "dht11.h"
#include <Adafruit_BMP085.h>
#include "anemometer.h"
#include "json.h"
#include "ntp.h"
#include "config.h"

Adafruit_BMP085 bmp;
bool bmpPresent;
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
    bmpPresent = false;
	}
  else{
    Serial.println("BMP found!");
    bmpPresent = true;
  }

  Serial.println("Setup done");
  //enter wifi details.
  initWiFi(WIFI_SSID, WIFI_PASSWORD);
  initNTP();
}

void readSensors() {
  if(WiFi.status() == WL_CONNECTED){
    dateTime = getDateTime();
  }
  dhtReadSensor();
  temp = getTemp();
  humidity = getHumidity();
  percievedTemp = getPercievedTemp();
  if(bmpPresent){
    pressure = bmp.readPressure();
    altitude = bmp.readAltitude();
    bmpTemp = bmp.readTemperature();
  }
}

void outputDebug() {
  Serial.println();
  Serial.println("Date: " + dateTime);
  Serial.println(("Temp: " + std::to_string(temp)).c_str());
  Serial.println(("Humidity: " + std::to_string(humidity)).c_str());
  Serial.println(("Feels like: " + std::to_string(percievedTemp)).c_str());
  if(HAS_BMP){
    Serial.println(("Altitude: " + std::to_string(altitude)).c_str());
    Serial.println(("Pressure: " + std::to_string(pressure)).c_str());
  }
}

void makeJson(Json& jsonData){
  jsonData.addValue("Date", ("\"" + dateTime + "\"").c_str());
  jsonData.addValue("Temperature", std::to_string(temp));
  jsonData.addValue("humidity", std::to_string(humidity));
  jsonData.addValue("percievedTemp", std::to_string(percievedTemp));
  if(HAS_BMP){
    jsonData.addValue("altitude", std::to_string(altitude));
    jsonData.addValue("pressure", std::to_string(pressure));
  }
}

void loop() {
  readSensors();
  if(DEBUG) outputDebug();  
  
  Json testData = Json(STATION_NAME);
  makeJson(testData);
  //Serial.println(testData.returnJson().c_str());
  if(WiFi.status() == WL_CONNECTED){
    if(checkCondition(SERVER_ADDRESS)){
      Serial.println("Server wants data");
      sendJson(SERVER_ADDRESS, testData.returnJson().c_str());
    }
    else{Serial.println("Server dont want data");}
  }
  delay(10000);
}
