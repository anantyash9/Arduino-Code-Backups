/*     Arduino Color Sensing Tutorial
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */

#include "I2Cdev.h" 
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL

#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}




#define S0 50           //previous - 4
#define S1 51           //previous - 5
#define S2 48           //previous - 9
#define S3 49           //previous - 10
#define sensorOut1 41   //previous - 8
#define sensorOut2 42   //previous - 9
#define sensorOut3 43   //previous - 10
#define sensorOut4 44
#define sensorOut5 45
#define sensorOut6 46
#define sensorOut7 47


#define lMotor1 25
#define lMotor2 24
#define lMotorPWM 8

#define rMotor1 23
#define rMotor2 22
#define rMotorPWM 9

#define triggerPin 6
#define echoPin 5

int frequencyRed[7] = {
  0};
int frequencyBlue[7] = {
  0};
int frequencyGreen[7] = {
  0};
int state;
double cm;
int effect= 0;

float *hsvM = new float[3];
float *hsvR = new float[3];
float *hsvER = new float[3];
float *hsvEER = new float[3];
float *hsvL = new float[3];
float *hsvEL = new float[3];
float *hsvEEL = new float[3];

int dir, pwmL, pwmR, dl, hueCount = 0;

void setup() {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  Serial.begin(115200);
  while (!Serial);
  mpu.initialize();
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(14);
  mpu.setYGyroOffset(-74);
  mpu.setZGyroOffset(-27);
  mpu.setXAccelOffset(-735);
  mpu.setYAccelOffset(1282);
  mpu.setZAccelOffset(516);
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    mpu.setDMPEnabled(true);
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    dmpReady = true;

    packetSize = mpu.dmpGetFIFOPacketSize();
  }
  pinMode(rMotor1, OUTPUT);
  pinMode(rMotor2, OUTPUT);
  pinMode(rMotorPWM, OUTPUT);

  pinMode(lMotor1, OUTPUT);
  pinMode(lMotor2, OUTPUT);
  pinMode(lMotorPWM, OUTPUT);  

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut1, INPUT);
  pinMode(sensorOut2, INPUT);
  pinMode(sensorOut3, INPUT);
  pinMode(sensorOut4, INPUT);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

}

boolean useMapping = true;
boolean printSensor1 = false;
boolean printSensor2 = false;
boolean printSensor3 = false;
boolean printSensor4 = false;
boolean printSensor5 = false;
boolean printSensor6 = false;
boolean printSensor7 = false;
boolean reached= false;
boolean reached2= false;
boolean turned = false;

//Move Robot
void moveBot(int dir, int Lpwm, int Rpwm){
  analogWrite(lMotorPWM, Lpwm);
  analogWrite(rMotorPWM, Rpwm);
  delay(10);

  switch(dir){
  case 1: 
    digitalWrite(lMotor1, HIGH);
    digitalWrite(lMotor2, LOW);
    digitalWrite(rMotor1, HIGH);
    digitalWrite(rMotor2, LOW);
    break;

  case 2: 
    digitalWrite(lMotor1, LOW);
    digitalWrite(lMotor2, HIGH);
    digitalWrite(rMotor1, LOW);
    digitalWrite(rMotor2, HIGH);
    break;

  case 3: 
    digitalWrite(lMotor1, LOW);
    digitalWrite(lMotor2, HIGH);
    digitalWrite(rMotor1, HIGH);
    digitalWrite(rMotor2, LOW);
    break;

  case 4: 
    digitalWrite(lMotor1, HIGH);
    digitalWrite(lMotor2, LOW);
    digitalWrite(rMotor1, LOW);
    digitalWrite(rMotor2, HIGH);
    break;

  default:
    digitalWrite(lMotor1, LOW);
    digitalWrite(lMotor2, LOW);
    digitalWrite(rMotor1, LOW);
    digitalWrite(rMotor2, LOW);
    analogWrite(lMotorPWM, 0);
    analogWrite(rMotorPWM, 0);
  }
}

//RGB2HSV conversion and helper functions
float fract(float x) { 
  return x - int(x); 
}

float mix(float a, float b, float t) { 
  return a + (b - a) * t; 
}

float step(float e, float x) { 
  return x < e ? 0.0 : 1.0; 
}

void rgb2hsv(int r, int g, int b, float* hsv) {
  float s = step(b, g);
  float px = mix(b, g, s);
  float py = mix(g, b, s);
  float pz = mix(-1.0, 0.0, s);
  float pw = mix(0.6666666, -0.3333333, s);
  s = step(px, r);
  float qx = mix(px, r, s);
  float qz = mix(pw, pz, s);
  float qw = mix(r, px, s);
  float d = qx - min(qw, py);
  hsv[0] = abs(qz + (qw - py) / (6.0 * d + 1e-10));
  hsv[1] = d / (qx + 1e-10);
  hsv[2] = qx;
}

double microsecondsToCM(long duration){
  return (double)duration/58 ;
}

double readUltraSonic(){
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 11600);
  double cm = microsecondsToCM(duration);
  if(duration == 0){
    cm = 200;
  }
  return cm;
}

//Main function


void getSensorValues()
{
  cm = readUltraSonic();

  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequencyRed[0] = pulseIn(sensorOut1, LOW);
  frequencyRed[1] = pulseIn(sensorOut2, LOW);
  frequencyRed[2] = pulseIn(sensorOut3, LOW);
  frequencyRed[3] = pulseIn(sensorOut4, LOW);
  frequencyRed[4] = pulseIn(sensorOut5, LOW);
  frequencyRed[5] = pulseIn(sensorOut6, LOW);
  frequencyRed[6] = pulseIn(sensorOut7, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  if(useMapping){
    frequencyRed[0] = map(frequencyRed[0], 289,1902,255,0);
    frequencyRed[1] = map(frequencyRed[1], 262,1923,255,0);
    frequencyRed[2] = map(frequencyRed[2], 287,2048,255,0);
    frequencyRed[3] = map(frequencyRed[3], 256,1835,255,0);
    frequencyRed[4] = map(frequencyRed[4], 260,1984,255,0);
    frequencyRed[5] = map(frequencyRed[5], 247,2026,255,0);
    frequencyRed[6] = map(frequencyRed[6], 279,2265,255,0);
  }
  delay(10);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequencyGreen[0] = pulseIn(sensorOut1, LOW);
  frequencyGreen[1] = pulseIn(sensorOut2, LOW);
  frequencyGreen[2] = pulseIn(sensorOut3, LOW);
  frequencyGreen[3] = pulseIn(sensorOut4, LOW);
  frequencyGreen[4] = pulseIn(sensorOut5, LOW);
  frequencyGreen[5] = pulseIn(sensorOut6, LOW);
  frequencyGreen[6] = pulseIn(sensorOut7, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  if(useMapping){
    frequencyGreen[0] = map(frequencyGreen[0], 258,1786,255,0);
    frequencyGreen[1] = map(frequencyGreen[1], 254,1862,255,0);
    frequencyGreen[2] = map(frequencyGreen[2], 264,1942,255,0);
    frequencyGreen[3] = map(frequencyGreen[3], 245,1787,255,0);
    frequencyGreen[4] = map(frequencyGreen[4], 245,1887,255,0);
    frequencyGreen[5] = map(frequencyGreen[5], 225,1843,255,0);
    frequencyGreen[6] = map(frequencyGreen[6], 259,2110,255,0);
  }

  delay(10);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequencyBlue[0] = pulseIn(sensorOut1, LOW);
  frequencyBlue[1] = pulseIn(sensorOut2, LOW);
  frequencyBlue[2] = pulseIn(sensorOut3, LOW);
  frequencyBlue[3] = pulseIn(sensorOut4, LOW);
  frequencyBlue[4] = pulseIn(sensorOut5, LOW);
  frequencyBlue[5] = pulseIn(sensorOut6, LOW);
  frequencyBlue[6] = pulseIn(sensorOut7, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  if(useMapping){
    frequencyBlue[0] = map(frequencyBlue[0], 208,1380,255,0);
    frequencyBlue[1] = map(frequencyBlue[1], 197,1385,255,0);
    frequencyBlue[2] = map(frequencyBlue[2], 216,1506,255,0);
    frequencyBlue[3] = map(frequencyBlue[3], 188,1322,255,0);
    frequencyBlue[4] = map(frequencyBlue[4], 180,1417,255,0);
    frequencyBlue[5] = map(frequencyBlue[5], 175,1397,255,0);
    frequencyBlue[6] = map(frequencyBlue[6], 195,1576,255,0);
  }
  if(printSensor1){
    // Printing the value on the serial monitor
    Serial.print("EER: r");//printing name
    Serial.print(frequencyRed[0]);
    Serial.print(", g");
    Serial.print(frequencyGreen[0]);
    Serial.print(", b");
    Serial.print(frequencyBlue[0]);
    Serial.print(" ");
  }
  if(printSensor2){
    // Printing the value on the serial monitor
    Serial.print("ER: r");//printing name
    Serial.print(frequencyRed[1]);
    Serial.print(", g");
    Serial.print(frequencyGreen[1]);
    Serial.print(", b");
    Serial.print(frequencyBlue[1]);
    Serial.print(" ");
  }
  if(printSensor3){
    // Printing the value on the serial monitor
    Serial.print("R: r");//printing name
    Serial.print(frequencyRed[2]);//printing RED color frequency
    Serial.print(", g");
    Serial.print(frequencyGreen[2]);
    Serial.print(", ");
    Serial.print(frequencyBlue[2]);
    Serial.print(" b");
  }
  if(printSensor4){
    // Printing the value on the serial monitor
    Serial.print("M: r");//printing name
    Serial.print(frequencyRed[3]);//printing RED color frequency
    Serial.print(", g");
    Serial.print(frequencyGreen[3]);
    Serial.print(", b");
    Serial.print(frequencyBlue[3]);
    Serial.print(" ");
  } 
  if(printSensor5){
    // Printing the value on the serial monitor
    Serial.print("L: r");//printing name
    Serial.print(frequencyRed[4]);
    Serial.print(", g");
    Serial.print(frequencyGreen[4]);
    Serial.print(", b");
    Serial.print(frequencyBlue[4]);
    Serial.print(" ");
  }
  if(printSensor6){
    // Printing the value on the serial monitor
    Serial.print("EL: r");//printing name
    Serial.print(frequencyRed[5]);//printing RED color frequency
    Serial.print(", g");
    Serial.print(frequencyGreen[5]);
    Serial.print(", b");
    Serial.print(frequencyBlue[5]);
    Serial.print(" ");
  }
  if(printSensor7){
    // Printing the value on the serial monitor
    Serial.print("EEL: r");//printing name
    Serial.print(frequencyRed[6]);//printing RED color frequency
    Serial.print(", g");
    Serial.print(frequencyGreen[6]);
    Serial.print(", b");
    Serial.print(frequencyBlue[6]);
    Serial.println(" ");
  } 
  delay(10);

  rgb2hsv(frequencyRed[0], frequencyGreen[0], frequencyBlue[0], hsvEER);
  rgb2hsv(frequencyRed[1], frequencyGreen[1], frequencyBlue[1], hsvER);
  rgb2hsv(frequencyRed[2], frequencyGreen[2], frequencyBlue[2], hsvR);
  rgb2hsv(frequencyRed[3], frequencyGreen[3], frequencyBlue[3], hsvM);
  rgb2hsv(frequencyRed[4], frequencyGreen[4], frequencyBlue[4], hsvL);
  rgb2hsv(frequencyRed[5], frequencyGreen[5], frequencyBlue[5], hsvEL);
  rgb2hsv(frequencyRed[6], frequencyGreen[6], frequencyBlue[6], hsvEEL);
  int layer=0;
  Serial.print(hsvEER[layer]);
  Serial.print(", ");
  Serial.print(hsvER[layer]);
  Serial.print(", ");
  Serial.print(hsvR[layer]);
  Serial.print(", ");
  Serial.print(hsvM[layer]);
  Serial.print(", ");
  Serial.print(hsvL[layer]);
  Serial.print(", ");
  Serial.print(hsvEL[layer]);
  Serial.print(", ");
  Serial.println(hsvEEL[layer]);
}

int getHeading() {
  // if programming failed, don't try to do anything
  if (!dmpReady) return 250;

  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    //    Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } 
  else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;


#ifdef OUTPUT_READABLE_YAWPITCHROLL
    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    //Serial.print(ypr[0] * 180 / M_PI);
    return (ypr[0] * 180 / M_PI);
#endif
  }
  return 250;
}
int getAngle()
{
  int temp=250;
  while (abs(temp)>180){
    Serial.print("Temp Heading: ");
    Serial.println(temp);
    temp=getHeading();
  }
  return temp;

}

bool getTarget(int initial,int currentAngle, int turnAngle)
{
  int final = initial+turnAngle;
  if (final < -180)
  {
    final = final + 360;
  } 
  else if(final > 180)
  {
    final =-360 + final;
  }
  Serial.print("target angle ");
  Serial.println(final);
  if (final+10 >180){
    //Serial.println("  target near to 180 ");
    bool inRange = ((currentAngle > final - 10) || (currentAngle < final - 350));
    return inRange;
  }
  else if(final-10 < -180){
    bool inRange = ((currentAngle > final + 350) || (currentAngle < final + 10));
    //Serial.print("Within the range: ");
    //Serial.println(inRange);
    return inRange;
  }
  else{
    bool inRange = ((currentAngle > final - 10) && (currentAngle < final+10));
    return inRange;
  }
}
int getSerial(){
  if(Serial.available()){
    return Serial.read() - '0';
  }

}
void followLine()
{
  while(!reached){
    getSensorValues();  
    dir = 5;
    pwmL = 128;
    pwmR = 128;
    dl = 1;

    if(hsvEER[2] < 230){
      //Serial.println("Trun right.... right now...");
      dir = 4;
      dl = 50;
    }
    else if(hsvEEL[2] < 230){
      //Serial.println("Trun left......right now...");
      dir = 3;
      dl  = 50;
    }
    else if(hsvER[2] < 230){
      //Serial.println("Trun right...");
      dir = 4;
      pwmL = 210;
      pwmR = 64;
      dl = 50;
    }
    else if(hsvEL[2] < 230){
      //Serial.println("Turn left.....");
      dir = 3;
      pwmL = 64;
      pwmR = 210;
      dl = 50;
    }
    else if(hsvR[2] < 230){
      //Serial.println("Trun right....aaram se...");
      dir = 1;
      pwmR = 0;
      pwmL = 128;
      dl = 10;
    }
    else if(hsvL[2] < 230){
      //Serial.println("Turn left.... aaram se...");
      dir = 1;
      pwmL = 0;
      pwmR = 128;
      dl = 10;
    }
    else if(hsvM[2] < 230){
      //Serial.println("Move staright....");
      dir = 1;
      dl = 10;
    }
//    if((hsvEER[0] > 0.60 && hsvEER[0] < 0.65) || (hsvER[0] > 0.60 && hsvER[0] < 0.65) || (hsvR[0] > 0.6 && hsvR[0] < 0.65) || 
//      (hsvEEL[0] > 0.60 && hsvEEL[0] < 0.65) || (hsvEL[0] > 0.60 && hsvEL[0] < 0.65) || (hsvL[0] > 0.60 && hsvL[0] < 0.65) || 
//      (hsvM[0] > 0.60 && hsvM[0] < 0.65)){
      if (hsvEER[0] > 0.60 && hsvEER[0] < 0.65)
        hueCount++;
      if (hsvER[0] > 0.60 && hsvEER[0] < 0.65)
        hueCount++;
      if (hsvR[0] > 0.60 && hsvEER[0] < 0.65)
        hueCount++;
      if (hsvM[0] > 0.60 && hsvEER[0] < 0.65)
        hueCount++;
      if (hsvL[0] > 0.60 && hsvEER[0] < 0.65)
        hueCount++;
      if (hsvEL[0] > 0.60 && hsvEER[0] < 0.65)
        hueCount++;
      if (hsvEEL[0] > 0.60 && hsvEER[0] < 0.65)
        hueCount++;  
      Serial.println("Counting hues");
    

    if(hueCount > 5){
      Serial.println("STOP!!");
      dir=5;
      reached =true;
    }
    hueCount=0;

    if(cm < 40){
      Serial.println("Obstacle!!");
      dir = 5;  
    }
    moveBot(dir,pwmL,pwmR);
   if (reached==true){
   
   return;} 
  }
}

void turnAround(int turnAngle)
{
  int initialAngle = getAngle();
  int currentAngle = initialAngle;
  //int targetAngle = getTarget(initialAngle);
  while (true)
  {
    dir = 4;
    pwmL = 200;
    pwmR = 200;
    dl = 10;
    moveBot(dir,pwmL,pwmR);
    currentAngle=getAngle();
    Serial.print("current angle ");
    Serial.print(currentAngle);
    Serial. print("   ");
    bool inRange = getTarget(initialAngle, currentAngle, turnAngle);
    Serial.print("In range: ");
    Serial.println(inRange);
    if(inRange == true)
      break;
  }
  Serial.println("out of the loop");
  moveBot(5, 0, 0);
  delay(10);
}
void go_straight()
  {
    moveBot(1,200,200);
  }
void loop() {
  while(Serial.available() == 0) { }
  state = Serial.parseInt();
  Serial.flush();
  Serial.println(state);

  if (state==1){
    Serial.println("Entering line following mode");
    go_straight();
  }
   else if (state==2){
    Serial.println("Turning");
    turnAround(90);
//    followLine();
    //turnAround();
    state=3;
    Serial.println("Done");
  }
}

