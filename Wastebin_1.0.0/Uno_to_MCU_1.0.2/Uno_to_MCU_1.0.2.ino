#include <SoftwareSerial.h>
#include <DS18B20.h>
#include <OneWire.h>
#include "HX711.h"
#include <MsTimer2.h>
#include <Servo.h>

SoftwareSerial uno(3,2);
DS18B20 tempSensor(8);
HX711 scale;
Servo pirServo;
Servo deoServo;

// String temp = "20"; //default temperature
// String tempUser;
// String weight = "3"; // default weight
// String weightUser;
String term = "10"; //default 10min
String termUser;
unsigned long ms = term.toInt() * 60000; // 60 * 1000ms = 1min

float sensorVal;
float scaleVal;

int deoServoPin = 7;
int pos;

//---------------------------------------------------------------pirServo start
int pirPin = 5;//HR-SR501 pin #5
//GND -> (-), Output ->(5), VCC ->(+) (초,보,파)
int binServoPin = 6;//Servo moter pin #6
//주황 ->6 , 빨강 -> +, 갈색 -> -
int pirPos = 0;
//---------------------------------------------------------------pirServo end


void setup() {
  Serial.begin(9600);
  uno.begin(9600);

  //1000ms = 1s
  MsTimer2::set(ms, runDeoServo);
  MsTimer2::start();
  scale.begin(10,11);

  scale.set_scale(2280.f);    //무게 0으로 리셋
  scale.tare(); 

  pirServo.attach(binServoPin);//servo pin #6
  pinMode(pirPin,INPUT);//PIR센서의 값을 읽을 디지털 핀을 7번으로
  deoServo.attach(deoServoPin);//deoServo pin #7         
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
  if(term != termUser){
    term = termUser;
    ms = term.toInt() * 60000;
    MsTimer2::set(ms, runDeoServo);
    MsTimer2::start();
  }
}

void loop() {
  sensorVal = tempSensor.getTempC();
  uno.println(sensorVal + '\n');
  Serial.println(sensorVal + '\n');

  scaleVal = scale.get_units()-10;
  uno.println(scaleVal + '\n');
  Serial.println(scaleVal + '\n');

  GetFromMCU();
  runPIRServo();

  delay(2000);
}

void runPIRServo(){
  int detect = digitalRead(pirPin);
  
  //HIGH(=1) -> PIR센서 작동
  //감지 -> 열림
  if(detect == 1){
    for(pirPos = 0; pirPos <= 150; pirPos += 1) //Servo
    { 
      pirServo.write(pirPos);
      delay(5); //delay값을 조정하여 모터의 속도를 컨터롤가능
    } 
    delay(5000); //열린 상태로 5초 지속
    for(pirPos = 150; pirPos>=1; pirPos-=1)
    { 
      pirServo.write(pirPos); 
      delay(15); 
    }
  }
  else{//감지X -> 닫힘
    pirServo.write(0);
    delay(1000);
  }
}

void runDeoServo(){
  for(pos = 0; pos <= 120; pos += 1) //Servo
  { 
    deoServo.write(pos);
    delay(15); //delay값을 조정하여 모터의 속도를 컨터롤가능
  } 
  
  delay(4000); //열린 상태로 5초 지속

  for(pos = 120; pos>=1; pos-=1)
  { 
    deoServo.write(pos); 
    delay(5); 
  }
  delay(1000);
}
