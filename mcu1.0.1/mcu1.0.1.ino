#include <ArduinoJson.h> //Version 5.13.5
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char* ssid     = "KT_GiGA_2G_Wave2_B232";
const char* password = "aecajee650";
const char* host = "121.191.167.72";
WiFiServer server(80);
WiFiClient client;
SoftwareSerial mcu(D6, D5);

const char* flag;
float temp;
int motorCycle = 30; //Default 30min
String response;

void setup() {
  Serial.begin(115200);
  mcu.begin(9600);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  client = server.available();

  //getDataFromMobius();
  //servoFlag();
  //WriteTempData();
  
}
 
void loop() {
  getDataFromUno();
  getDataFromMobius(); 
}

void getDataFromUno(){
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& data = jsonBuffer.parseObject(mcu);

    Serial.print("Recieved temp: ");
    temp = data["temp"];
    Serial.println(temp);
    delay(2000);
}
void runServo(){
  Serial.println("Motor Running");
  
}
//void servoFlag(){
//    HTTPClient http;
//    String body;
//    
//    if(temp > 20){
//      http.begin(client,"http://121.191.167.72:7579/Mobius/test/servo");
//
//      http.addHeader("Accept","application/json");
//      http.addHeader("X-M2M-RI", "12345");
//      http.addHeader("X-M2M-Origin", "S");
//      http.addHeader("Content-Type", "application/json;ty=4");
//
//      body = "{\"m2m:cin\": {\"con\": \"on\"}}";
//    }
//    else{
//      http.begin(client,"http://121.191.167.72:7579/Mobius/test/servo");
//
//      http.addHeader("Accept","application/json");
//      http.addHeader("X-M2M-RI", "12345");
//      http.addHeader("X-M2M-Origin", "S");
//      http.addHeader("Content-Type", "application/json;ty=4");
//
//      body = "{\"m2m:cin\": {\"con\": \"off\"}}";
//    }
//    int httpCode = http.POST(body);
//    const char* ans = getData("servo");
//    uno.begin(9600);
//    const char* ans = "on";
//    StaticJsonBuffer<1000> jsonBuffer;
//    JsonObject& data = jsonBuffer.createObject();
// 
//    data["flag"] = ans;
//    data.printTo(uno);
//    jsonBuffer.clear();
//    
//}

void WriteTempData(){
  HTTPClient http;
  http.begin(client,"http://121.191.167.72:7579/Mobius/test/tempData");

  http.addHeader("Accept","application/json");
  http.addHeader("X-M2M-RI", "12345");
  http.addHeader("X-M2M-Origin", "S");
  http.addHeader("Content-Type", "application/json;ty=4");

  //temp = getTempFromUno();
  String body = "{\"m2m:cin\": {\"con\": \"" + (String)temp + "\"}}";
  
  int httpCode = http.POST(body);

  Serial.println(httpCode);
  http.end();
}

void getDataFromMobius(){
  HTTPClient http;

  http.begin(client,"http://121.191.167.72:7579/Mobius/test/servo/latest"); // path
  
  //Definition Header
  http.addHeader("Accept","application/json");
  http.addHeader("X-M2M-RI", "12345");
  http.addHeader("X-M2M-Origin", "S");

  int httpCode = http.GET();
 
  if(httpCode > 0){
    response = http.getString();
    
    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(11) + 180;
    DynamicJsonBuffer jsonBuffer(capacity);

    JsonObject& root = jsonBuffer.parseObject(response);
    JsonObject& m2m_cin = root["m2m:cin"];
    
    flag = m2m_cin["con"]; // "27"
    Serial.print("Flag is "); Serial.println(flag);
  }

  http.begin(client,"http://121.191.167.72:7579/Mobius/test/defaultMotorCycle/latest"); // path
  
  //Definition Header
  http.addHeader("Accept","application/json");
  http.addHeader("X-M2M-RI", "12345");
  http.addHeader("X-M2M-Origin", "S");

  httpCode = http.GET();
 
  if(httpCode > 0){
    response = http.getString();
    
    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(11) + 180;
    DynamicJsonBuffer jsonBuffer(capacity);

    JsonObject& root = jsonBuffer.parseObject(response);
    JsonObject& m2m_cin = root["m2m:cin"];
    Serial.print("Default vaule: "); Serial.println(motorCycle);
    if(motorCycle != m2m_cin["con"]){
      motorCycle = m2m_cin["con"];
      runServo();
    }
    
  }
  http.end();

}
