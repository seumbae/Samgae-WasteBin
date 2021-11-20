#include<OneWire.h>
#include<DS18B20.h>

DS18B20 DS18B20_Sensor(2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(DS18B20_Sensor.getTempC());
  delay(100);
}
