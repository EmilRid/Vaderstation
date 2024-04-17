#include <Arduino.h>
#include <Wifi.h>
#include <HTTPclient.h>
#include "dht11.h"

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

  Serial.println("Setup done");
  //enter wifi details.
  initWiFi("ssid", "password");
}

void loop() {
  String serverName = "http://172.20.10.9:1880/";
  
  readSensor();
  char str[20];
  sprintf(str, "T:%d", getTemp());
  Serial.println(str);
  sprintf(str, "H:%d", getHumidity());
  Serial.println(str);
  sprintf(str, "PT:%d", getPercievedTemp());
  Serial.println(str);
  delay(10000);

  char testData[200];
  sprintf(testData, "{\"date\":\"1970-01-01T00:00:00Z\",\"temp\":\"%d\",\"humidity\":\"%d\",\"percievedTemp\":\"%d\"}", getTemp(), getHumidity(), getPercievedTemp());
  if(WiFi.status() == WL_CONNECTED){
    sendJson(serverName, testData);
  }
}
