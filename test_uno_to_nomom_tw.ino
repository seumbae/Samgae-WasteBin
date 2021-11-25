//Uno to NodeMCU
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <DS18B20.h>
#include <OneWire.h>
#include "HX711.h"

SoftwareSerial mcu(5,6);
DS18B20 DS18B20_Sensor(4);
HX711 scale;


void setup() {
  Serial.begin(9600);

  mcu.begin(9600);

  scale.begin(2,3);

  scale.set_scale(2280.f); //this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare(); // reset the scale to 0
}

void loop() {
  DynamicJsonBuffer jsonBuffer(1000);
  JsonObject& data = jsonBuffer.createObject();

  data["temp"] = DS18B20_Sensor.getTempC();
  data["weight"] = scale.get_units(10); //꼭 10을 넣어줘야함...
  data.printTo(mcu);
  jsonBuffer.clear();
  delay(2000);
}
