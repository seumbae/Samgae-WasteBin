#include <SoftwareSerial.h>
#include <DS18B20.h>
#include <OneWire.h>
#include "HX711.h"

SoftwareSerial uno(3,2);
DS18B20 sensor(7);
HX711 scale;

String tempUser;
String weightUser;
String termUser;

void setup() {
  Serial.begin(9600);
  uno.begin(9600);

  scale.begin(10,11);

  scale.set_scale(2280.f);    //무게 0으로 리셋
  scale.tare();               

}

//MCU정보 받기 UNO로
void GetFromMCU(){
  tempUser = uno.readStringUntil('\n');
  Serial.print("Recieved tempUser: ");
  Serial.println(tempUser);

  weightUser = uno.readStringUntil('\n');
  Serial.print("Recieved weightUser: ");
  Serial.println(weightUser);

  termUser = uno.readStringUntil('\n');
  Serial.print("Recieved termUser: ");
  Serial.println(termUser);
}

void loop() {
  float sensorVal = sensor.getTempC();
  uno.println(sensorVal + '\n');
  Serial.println(sensorVal + '\n');

  float scaleVal = scale.get_units()-10;
  uno.println(scaleVal + '\n');
  Serial.println(scaleVal + '\n');

  GetFromMCU();

  delay(2000);
}
