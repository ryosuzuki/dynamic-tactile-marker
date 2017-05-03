#include <ArduinoJson.h>
#include <Shifty.h>
Shifty shiftP;
Shifty shiftN;

int pData= 8;
int pLatch= 9;
int pClock= 10;

int nData= 2;
int nLatch = 3;
int nClock = 4;

void standby();
void turnOn();
void turnOff();

void setup() {
  shiftP.setBitCount(16);
  shiftP.setPins(pData, pClock, pLatch);
  shiftN.setBitCount(16);
  shiftN.setPins(nData, nClock, nLatch);
  Serial.begin(9600);
  standby();
}

void loop() {
  String json = "";
  while (Serial.available() > 0) {
    json += (char) Serial.read();
    delay(5);
  }

  if (json != "") {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(json);
    int p = root["p"];
    int n = root["n"];
    activate(p, n);
  }
}

void activate(int n, int p) {
  turnOn(p, n);
  delay(300);
  turnOff(p, n);
}


void standby() {
  shiftP.batchWriteBegin();
  shiftN.batchWriteBegin();
  for (int i=0; i<16; i++) {
    shiftP.writeBit(i, LOW);
    shiftN.writeBit(i, HIGH);
  }
  shiftP.batchWriteEnd();
  shiftN.batchWriteEnd();
}

void turnOn(int p, int n) {
  shiftP.writeBit(p, HIGH);
  shiftN.writeBit(n, LOW);
}

void turnOff(int p, int n) {
  shiftP.writeBit(p, LOW);
  shiftN.writeBit(n, HIGH);
}



