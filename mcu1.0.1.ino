#include <ArduinoJson.h> //Version 5.13.5
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

 
const char* ssid     = "KT_GiGA_2G_Wave2_B232";
const char* password = "aecajee650";
const char* host = "121.191.167.72";

WiFiServer server(80);
WiFiClient client;

String response;
void setup() {
  Serial.begin(115200);
 
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

  getData();
  //createAE();
}
 
void loop() {
 
}

void getData(){
  HTTPClient http;
  http.begin(client,"http://121.191.167.72:7579/Mobius/test_ae1/sensorData/latest"); // path
  
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

  String body = "{\"m2m:ae\":{\"rn\": \"test_ae1\",\"api\": \"0.2.481.2.0001.001.000111\",\"lbl\": [\"key1\", \"key2\"],\"rr\": true,\"poa\": [\"http://203.254.173.104:9727\"]}}";
  int httpCode = http.POST(body); 

  Serial.println(httpCode);
  http.end();
}
