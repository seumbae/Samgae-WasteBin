//Uno to NodeMCU
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <DS18B20.h>
#include <OneWire.h>

#include <Servo.h>

SoftwareSerial mcu(5,6); //Rx, TX
DS18B20 DS18B20_Sensor(7);
float temp;

void setup() {
  Serial.begin(9600);

  mcu.begin(9600);
  //servo.attach(8);
}

void loop() {
  tempToMcu();
  //servoFlag();
  //servoSweep(flag);

}

void tempToMcu(){
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
 
  temp = DS18B20_Sensor.getTempC();
  data["temp"] = temp;
  //test
  data["weight"] = 10;
  data.printTo(mcu);
  Serial.println(temp);
  jsonBuffer.clear();
  delay(2000);
}
//void servoSweep(String flag){
//  int pos;
//  //servoFlag();
//  if(flag =="on"){
//    //open
//    for(pos = 0 ;pos <= 180; pos +=1){
//      servo.write(pos);
//    } 
//    delay(5000);
//    //close
//    for (pos = 180; pos >= 0; pos -=1) {
//      servo.write(pos);              
//    }
//  }
//}
//void servoFlag(){
//    uno.begin(9600);
//    StaticJsonBuffer<1000> jsonBuffer;
//    JsonObject& data = jsonBuffer.parseObject(uno);
//
//    Serial.print("Recieved flag: ");
//    flag = data["flag"];
//    Serial.println(flag);  
//    delay(3000);
//  
//}
