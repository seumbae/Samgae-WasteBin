#include <I2CTransfer.h>
#include <Packet.h>
#include <PacketCRC.h>
#include <SerialTransfer.h>
#include <SPITransfer.h>

SerialTransfer st;

struct ST{
  float temp;
  float weight;
}testST;

void setup() {
  Serial.begin(115200);
  st.begin(Serial);
}

void loop() {
  st.rxObj(testST, sizeof(testST));
  Serial.print("Received temp: ");
  Serial.println(testST.temp);
  Serial.print("Received weigth: ");
  Serial.println(testST.weight);
  delay(1000);
}
