#include <WiegandNG.h>
#include <WiegandNG1.h>

String buildBinary(WiegandNG &tempwg) {
  volatile unsigned char *buffer=tempwg.getRawData();
  unsigned int bufferSize = tempwg.getBufferSize();
  unsigned int countedBits = tempwg.getBitCounted();
  unsigned int countedBytes = (countedBits/8);
  if ((countedBits % 8)>0) countedBytes++;
  char buff[tempwg.getBitCounted()+1] = "";
  buff[tempwg.getBitCounted()] = '\0';
  int index = 0;
  for (unsigned int i=bufferSize-countedBytes; i< bufferSize;i++) {
    unsigned char bufByte=buffer[i];
    for(int x=0; x<8;x++) {
      if ( (((bufferSize-i) *8)-x) <= countedBits) {
        if((bufByte & 0x80)) {
          buff[index++]= '1';
        }
        else {
          buff[index++]= '0';
        }
      }
      bufByte<<=1;
    }
  }
  return String(buff);
}

String buildBinary(WiegandNG1 &tempwg) {
  volatile unsigned char *buffer=tempwg.getRawData();
  unsigned int bufferSize = tempwg.getBufferSize();
  unsigned int countedBits = tempwg.getBitCounted();
  unsigned int countedBytes = (countedBits/8);
  if ((countedBits % 8)>0) countedBytes++;
  char buff[tempwg.getBitCounted()+1] = "";
  buff[tempwg.getBitCounted()] = '\0';
  int index = 0;
  for (unsigned int i=bufferSize-countedBytes; i< bufferSize;i++) {
    unsigned char bufByte=buffer[i];
    for(int x=0; x<8;x++) {
      if ( (((bufferSize-i) *8)-x) <= countedBits) {
        if((bufByte & 0x80)) {
          buff[index++]= '1';
        }
        else {
          buff[index++]= '0';
        }
      }
      bufByte<<=1;
    }
  }
  return String(buff);
}

WiegandNG wgIN;
WiegandNG1 wgOUT;

int pinIND0 = 2;
int pinIND1 = 3;

int pinOUTD0 = 19;
int pinOUTD1 = 20;

int fPin = 22;
int rPin = 23;

void setup() {
  Serial.begin(9600);
  unsigned int wiegandbits = 48;
  unsigned int packetGap = 15;  
  if(!wgIN.begin(pinIND0, pinIND1, wiegandbits, packetGap)) {
    Serial.println("00000000000000000000000000000000000");
  }
  if(!wgOUT.begin(pinOUTD0, pinOUTD1, wiegandbits, packetGap)) {
    Serial.println("00000000000000000000000000000000000");
  }

  pinMode(fPin, OUTPUT);
  pinMode(rPin, OUTPUT);
}

void loop() {
  if(wgIN.available()) {
    wgIN.pause();
    Serial.println("#"+buildBinary(wgIN)+"00");
    wgIN.clear();
  }
  if(wgOUT.available()) {
    wgOUT.pause();
    Serial.println("#"+buildBinary(wgOUT)+"01");
    wgOUT.clear();
  }
  if (Serial.available() > 0){
    switch((char)Serial.read()){
      case '0':
        Serial.println("@fPin");
        digitalWrite(fPin, HIGH);
        break;
      case '1':
        Serial.println("@rPin");
        digitalWrite(rPin, HIGH);
        break;
      default:
        Serial.println("@-");
    }
    delay(100);
    digitalWrite(fPin, LOW);
    digitalWrite(rPin, LOW);
  }
}
