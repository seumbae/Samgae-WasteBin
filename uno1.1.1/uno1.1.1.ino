#include "SerialTransfer.h"
#include <DS18B20.h>

SerialTransfer st;
DS18B20 DS18B20_Sensor(7);

struct ST{
  float temp;
  float weight = 10;
} testST;

void setup() {
  Serial.begin(115200);
  st.begin(Serial);
}

void loop() {
  testST.temp = DS18B20_Sensor.getTempC();
  st.txObj(testST, sizeof(testST));
  st.sendData(sizeof(testST));
  Serial.println(testST.temp);
  delay(1000);
}
