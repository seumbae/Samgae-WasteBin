#include <SoftwareSerial.h>
#include <DS18B20.h>
#include <OneWire.h>
#include "HX711.h"
#include <MsTimer2.h>

SoftwareSerial uno(3,2);
DS18B20 sensor(7);
HX711 scale;


// String temp = "20"; //default temperature
// String tempUser;
// String weight = "3"; // default weight
// String weightUser;
String term = "10"; //default 10min
String termUser;
unsigned int cycle = 1000 * 60; //1000*60 = 60s

float sensorVal;
float scaleVal;

void setup() {
  Serial.begin(9600);
  uno.begin(9600);

  //1000ms = 1s
  MsTimer2::set(termUser*ms, runServo);
  MsTimer2::start();
  scale.begin(10,11);

  scale.set_scale(2280.f);    //무게 0으로 리셋
  scale.tare();               

}

//MCU정보 받기 UNO로
void GetFromMCU(){
  // tempUser = uno.readStringUntil('\n');
  // Serial.print("Recieved tempUser: ");
  // Serial.println(tempUser);

  // weightUser = uno.readStringUntil('\n');
  // Serial.print("Recieved weightUser: ");
  // Serial.println(weightUser);

  termUser = uno.readStringUntil('\n');
  Serial.print("Recieved termUser: ");
  Serial.println(termUser);
  if(temp != tempUser){
    temp = tempUser;
    MsTimer2::set(term.toInt() * cycle, runServo);
    MsTimer2::start();
  }
}

void loop() {
  sensorVal = sensor.getTempC();
  uno.println(sensorVal + '\n');
  Serial.println(sensorVal + '\n');

  scaleVal = scale.get_units()-10;
  uno.println(scaleVal + '\n');
  Serial.println(scaleVal + '\n');

  GetFromMCU();

  delay(2000);
}

void runServo{
  // 너희가 짠 코드 있는것 같길래
}
