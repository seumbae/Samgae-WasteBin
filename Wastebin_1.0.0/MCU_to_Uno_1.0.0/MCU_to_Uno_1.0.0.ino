#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h> //Version 5.13.5

const char* ssid     = "Why314";
const char* password = "wellcome79*";
const char* host = "192.168.35.2";
String serverName = "http://192.168.35.2:7579/Mobius/test";
String Name1 = "tempData";
String Name2 = "weightData";

WiFiServer server(80);
WiFiClient client;

SoftwareSerial mcu(14,12);//D5 =14, D6 = 12

//POST
String response;
String temp; //float이었음
String weight; //float이었음

//GET
String tempUser; //사용자가 입력한 온도값
String weightUser; //사용자가 입력한 무게값
String termUser; //사용자 설정값
String response1; //상태코드1
String response2; //상태코드2
String response3; //상태코드3

void setup() {
  Serial.begin(9600);
  mcu.begin(9600);

  //--------------------------------------------------------wifi
  Serial.println();
  Serial.print("Connecting to");
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
  //--------------------------------------------------------wifi
}


//UNO정보 MCU로 받아오기
void GetFromUno(){

  temp = mcu.readStringUntil('\n');//개행 전까지 온도(temp) string으로 받아옴 from Uno
  Serial.print("Recieved temp: ");
  Serial.println(temp);

  weight = mcu.readStringUntil('\n');//개행 전까지 무게(weight) string으로 받아옴 from Uno
  Serial.print("Recieved weight: ");
  Serial.println(weight);
}

//UNO정보 MCU에서 Mobius로 보내기
void SendToMobi(){
  HTTPClient http;

  //--------------------------------------------------------temp
  http.begin(client,serverName + "/" + Name1); //tempData
  http.addHeader("Content-Type", "application/vnd.onem2m-res+xml;ty=4");
  http.addHeader("X-M2M-RI", "adnae/1234");
  http.addHeader("X-M2M-Origin", Name1);
  String httpRequestData1 = "";
  httpRequestData1 += "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\">\n";
  httpRequestData1 += "<con>" + temp + "</con>";
  httpRequestData1 += "</m2m:cin>";
          
  int httpResponseCode1 = http.POST(httpRequestData1);
 
  http.end();  
  //--------------------------------------------------------temp

  //--------------------------------------------------------weigh
  http.begin(client,serverName + "/" + Name2); //weightData
  http.addHeader("Content-Type", "application/vnd.onem2m-res+xml;ty=4");
  http.addHeader("X-M2M-RI", "adnae/1234");
  http.addHeader("X-M2M-Origin", Name2);
  String httpRequestData2 = "";
  httpRequestData2 += "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\">\n";
  httpRequestData2 += "<con>" + weight + "</con>";
  httpRequestData2 += "</m2m:cin>";
          
  int httpResponseCode2 = http.POST(httpRequestData2);
    
  http.end(); 
  //--------------------------------------------------------weigh
}

//Mobi정보 MCU로 받아오기
void GetFromMobi(){
  HTTPClient http;

  // //--------------------------------------------------------temp
  // http.begin(client,serverName + "/" + "tempUser" + "/" +"latest");
  // http.addHeader("X-M2M-RI", "1234");
  // http.addHeader("X-M2M-Origin", "tempUser");
      
  // int httpCode1 = http.GET();
  // if(httpCode1 > 0 ){
  //   response1 = http.getString();
  // }
  // const size_t capacity1 = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(11) + 180;
  // DynamicJsonBuffer jsonBuffer1(capacity1);

  // JsonObject& root1 = jsonBuffer1.parseObject(response1);
  // JsonObject& m2m_cin1 = root1["m2m:cin"];
    
  // tempUser = (String)m2m_cin1["con"];
  // http.end();
  // //--------------------------------------------------------temp

  // //--------------------------------------------------------weight
  // http.begin(client,serverName + "/" + "weightUser" + "/" + "latest");
  // http.addHeader("X-M2M-RI", "1234");
  // http.addHeader("X-M2M-Origin", "weightUser");

  // int httpCode2 = http.GET();
  // if(httpCode2>0){
  //   response2 = http.getString();
  // }
  // const size_t capacity2 = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(11) + 180;
  // DynamicJsonBuffer jsonBuffer2(capacity2);

  // JsonObject& root2 = jsonBuffer2.parseObject(response2);
  // JsonObject& m2m_cin2 = root2["m2m:cin"];
    
  // weightUser = (String)m2m_cin2["con"];
  // http.end();   
  // //--------------------------------------------------------weight

  //--------------------------------------------------------term
  http.begin(client,serverName + "/" + "termUser" + "/" + "latest");
  http.addHeader("X-M2M-RI", "1234");
  http.addHeader("X-M2M-Origin", "termUser");

  int httpCode3 = http.GET();

  if(httpCode3 > 0){
    response3 = http.getString();
  }
  const size_t capacity3 = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(11) + 180;
  DynamicJsonBuffer jsonBuffer3(capacity3);

  JsonObject& root3 = jsonBuffer3.parseObject(response3);
  JsonObject& m2m_cin3 = root3["m2m:cin"];
    
  termUser = (String)m2m_cin3["con"];
  http.end();  
  //--------------------------------------------------------term


}

//Mobi정보 MCU통해서 UNO로
void SendToUno(){
  // mcu.println(tempUser);//tempUser 자체가 String -> 전송
  // Serial.println(tempUser);
  
  // mcu.println(weightUser);
  // Serial.println(weightUser);
  
  mcu.println(termUser);
  Serial.println(termUser);
}


void loop() {
  GetFromUno();
  SendToMobi();

  GetFromMobi();
  SendToUno();
  delay(2000);
}
