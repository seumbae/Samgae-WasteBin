#include <ArduinoJson.h> //Version 5.13.5
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char* ssid     = "Why314";
const char* password = "wellcome79*";
const char* host = "192.168.35.2";

WiFiServer server(80);
WiFiClient client;

SoftwareSerial mcu(D6, D5);
String response;
float temp;
float weight;

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

  //getData(); //2. 왜인지는 모르겠지만 마지막으로 이거 주석달았더니 uno_to_nomom 데이터 전송 잘됨(이전에는 우노에서 노엠으로 전송이 안됐음)
  //문제는 위에 주석달으면 온도정보를 uno로 전송못함
  //getData();
  //createAE();
  //WriteTempData();
}

void loop() {
  getTempFromUno();
  WriteTempData();
}

void getTempFromUno() {
  DynamicJsonBuffer jsonBuffer(1000);
  JsonObject& data = jsonBuffer.parseObject(mcu);

  Serial.print("Recieved temp: ");
  temp = data["temp"];
  Serial.println(temp);

  Serial.print("Recived weight: ");
  weight = data["weight"];
  Serial.println(weight);

  delay(2000);
}

void WriteTempData() { //1. 전역변수 사용할꺼면 float -> void를 해야됐었음 -> 결과적으로 정보전송은 잘 되는데 uno to nom이 안되네.. 되다가 안됨 유실됨
  //getTempFromUno();
  HTTPClient http;

  //temp
  http.begin(client, "http://192.168.35.2:7579/Mobius/test/tempData");

  http.addHeader("Accept", "application/json");
  http.addHeader("X-M2M-RI", "12345");
  http.addHeader("X-M2M-Origin", "S");
  http.addHeader("Content-Type", "application/json;ty=4");

  String body_temp = "{\"m2m:cin\": {\"con\": \"" + (String)temp + "\"}}";

  int httpCode_temp = http.POST(body_temp);

  Serial.println(httpCode_temp);

  http.end();


  //weight
  http.begin(client, "http://192.168.35.2:7579/Mobius/test/weightData");

  http.addHeader("Accept", "application/json");
  http.addHeader("X-M2M-RI", "12345");
  http.addHeader("X-M2M-Origin", "S");
  http.addHeader("Content-Type", "application/json;ty=4");

  String body_weight = "{\"m2m:cin\": {\"con\": \"" + (String)weight + "\"}}";

  int httpCode_weight = http.POST(body_weight);

  Serial.println(httpCode_weight);

  http.end();
}

void getData() {
  HTTPClient http;
  http.begin(client, "http://192.168.35.2:7579/Mobius/test/tempData/latest"); // path

  //Definition Header
  http.addHeader("Accept", "application/json");
  http.addHeader("X-M2M-RI", "12345");
  http.addHeader("X-M2M-Origin", "S");
  int httpCode = http.GET();

  Serial.println(httpCode);
  if (httpCode > 0) {
    response = http.getString();

    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(11) + 180;
    DynamicJsonBuffer jsonBuffer(capacity);
    JsonObject& root = jsonBuffer.parseObject(response);
    JsonObject& m2m_cin = root["m2m:cin"];

    const char* data = m2m_cin["con"];
    Serial.println(data);
  }
  http.end();
}
