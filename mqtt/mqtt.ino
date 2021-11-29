#include <WiFiEsp.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// Update these with values suitable for your network.

const char* ssid = "KT_GiGA_2G_Wave2_B232";
const char* password = "aecajee650";
const char* mqtt_server = "http://127.0.0.1:7579";
const char* outTopic = "oneM2M/req/test/servo/Mobius2/json"; //defaultMotorCycle;
const char* inTopic = "oneM2M/resq/test/servo/Mobius2/json"; //servo
SoftwareSerial esp(2,3); //TX, RX

WiFiEspClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup() {
  esp.begin(9600);
  Serial.begin(9600);
  WiFi.init(&esp);
  initWifi();
  client.setServer(mqtt_server, 1883);
  publishMessageToServo();
  //client.publish(outTopic, "hello");
}

void initWifi(){
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
    Serial.println("WiFi connected");

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
}

void publishMessageToServo(){
  StaticJsonBuffer<100> root;
  JsonObject& con = root.createObject();
  JsonObject& cin = root.createObject();

  cin["cin"] = "HelloWorld";
  con["m2m:cin"] = cin["cin"];
  
  char output[128];
  con.printTo(output);

  client.publish(outTopic, output);
  Serial.println("End");
  
}

void loop() {
  
}
