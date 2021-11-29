//#include <ESP8266_Lib.h>
//
//#include "SerialTransfer.h"
//
//SerialTransfer st;
//WiFiServer server(80);
//
//const char* ssid     = "KT_GiGA_2G_Wave2_B232";
//const char* password = "aecajee650";
//
//struct ST{
//  float temp;
//  float weight;
//} testST;
//
//
//void setup() {
//  Serial.begin(115200);
//
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
//  server.begin();
//  st.begin(Serial);
//}
//
//void loop() 
//{
//  if(st.available())
//  {
//    st.rxObj(testST, sizeof(testST));
//    Serial.print("Received temp: "); Serial.println(testST.temp);
//    Serial.print("Received weight: "); Serial.println(testST.weight);
//    delay(1000);
//  } 
//}  

#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#define ESP8266_BAUD 9600

char auth[] = "6bae2d0e7bfa4b6096feeaffa2030e4f";
char ssid[] = "KT_GiGA_2G_Wave2_B232"; //You can replace the wifi name to your wifi 
char pass[] = "aecajee650";  //Type password of your wifi.

SoftwareSerial EspSerial(0, 1); // RX, TX

ESP8266 wifi(&EspSerial);

void setup()
{

  Serial.begin(9600);
  EspSerial.begin(ESP8266_BAUD);
  Blynk.begin(auth, wifi,ssid, pass);
 
}

void loop()
{
  Blynk.run();
}
