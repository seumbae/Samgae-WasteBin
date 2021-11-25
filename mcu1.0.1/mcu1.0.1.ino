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
String response;
float temp;

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

  //getData();
  //createAE();
  WriteTempData();
}
 
void loop() {
  //getTempFromUno();
}

float getTempFromUno(){
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(mcu);

  Serial.print("Recieved temp: ");
  float temp = data["temp"];
  Serial.println(temp);
  delay(2000);
  return temp;
}

void servo(){
    if(temp > 20){
      http.begin(client,"http://121.191.167.72:7579/Mobius/test/servo");

      http.addHeader("Accept","application/json");
      http.addHeader("X-M2M-RI", "12345");
      http.addHeader("X-M2M-Origin", "S");
      http.addHeader("Content-Type", "application/json;ty=4");

      String body = "{\"m2m:cin\": {\"con\": \"" + (String)temp + "\"}}";
    }
}

void WriteTempData(){
  HTTPClient http;
  http.begin(client,"http://121.191.167.72:7579/Mobius/test/tempData");

  http.addHeader("Accept","application/json");
  http.addHeader("X-M2M-RI", "12345");
  http.addHeader("X-M2M-Origin", "S");
  http.addHeader("Content-Type", "application/json;ty=4");

  temp = getTempFromUno();
  String body = "{\"m2m:cin\": {\"con\": \"" + (String)temp + "\"}}";
  
  int httpCode = http.POST(body);

  Serial.println(httpCode);
  http.end();
}

void getData(){
  HTTPClient http;
  http.begin(client,"http://121.191.167.72:7579/Mobius/test/tempData/latest"); // path
  
  //Definition Header
  http.addHeader("Accept","application/json");
  http.addHeader("X-M2M-RI", "12345");
  http.addHeader("X-M2M-Origin", "S");

  int httpCode = http.GET();
  
  Serial.println(httpCode);
  if(httpCode > 0){
    response = http.getString();
    
    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(11) + 180;
    DynamicJsonBuffer jsonBuffer(capacity);

    JsonObject& root = jsonBuffer.parseObject(response);
    JsonObject& m2m_cin = root["m2m:cin"];
    
    const char* data = m2m_cin["con"]; // "27"
    Serial.println(data);
  }
  http.end();

}

void createAE(){
  HTTPClient http;
  http.begin(client,"http://121.191.167.72:7579/Mobius"); // path
  
  //Definition Header
  http.addHeader("Accept","application/json");
  http.addHeader("X-M2M-RI", "12345");
  http.addHeader("X-M2M-Origin", "S");
  http.addHeader("Content-Type", "application/json;ty=2");

  String body = "{\"m2m:ae\":{\"rn\": \"test\",\"api\": \"0.2.481.2.0001.001.000111\",\"lbl\": [\"key1\", \"key2\"],\"rr\": true,\"poa\": [\"http://203.254.173.104:9727\"]}}";
  int httpCode = http.POST(body); 

  Serial.println(httpCode);
  http.end();
}
