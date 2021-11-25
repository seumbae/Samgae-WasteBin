//Uno to NodeMCU
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <DS18B20.h>
#include <OneWire.h>

SoftwareSerial mcu(5,6);
DS18B20 DS18B20_Sensor(7);

void setup() {
  Serial.begin(9600);

  mcu.begin(9600);
}

void loop() {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
 
  data["temp"] = DS18B20_Sensor.getTempC();
  data.printTo(mcu);
  jsonBuffer.clear();
  delay(2000);
}
