
// Wiring http://301o583r8shhildde3s0vcnh.wpengine.netdna-cdn.com/wp-content/uploads/2014/11/conn.png

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
// in "MPU6050_6Axis_MotionApps20.h"
// I'm now using the default value on line 305 to:  0x02,   0x16,   0x02,   0x00, 0x01                // D_0_22 inv_set_fifo_rate
// Correcting the PID code fixed my issues with the fifo buffer being too fast
#include "Wire.h"

MPU6050 mpu;

// These are my MPU6050 Offset numbers: for mpu.setXGyroOffset()
// supply your own gyro offsets here, scaled for min sensitivity use MPU6050_calibration.ino <<< download to calibrate your MPU6050 put the values the probram returns below
//                       XA      YA      ZA      XG      YG      ZG
int MPUOffsets[6] = {  
  210,	-1561,	1648,	62,	8,	28}; //MPU6050 on balanceing bot
//int MPUOffsets[6] = {-24640, 20392, 1784, -62.0, 19.0, 19.0};  //MPU9055 with pro mini


#define LED_PIN 13 // 

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}

// ================================================================
// ===                      MPU DMP SETUP                       ===
// ================================================================
int FifoAlive = 0; // tests if the interrupt is triggering
int IsAlive = -20;     // counts interrupt start at -20 to get 20+ good values before assuming connected
// MPU control/status vars
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
float Yaw, Pitch, Roll; // in degrees




#define lMotor1 22
#define lMotor2 23
#define lMotorPWM 9

#define rMotor1 24
#define rMotor2 25
#define rMotorPWM 8


int dir, pwmL, pwmR, dl;

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


void MPU6050Connect() {
  static int MPUInitCntr = 0;
  // initialize device
  mpu.initialize(); // same
  // load and configure the DMP
  devStatus = mpu.dmpInitialize();// same

    if (devStatus != 0) {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)

    char * StatStr[5] { 
      "No Error", "initial memory load failed", "DMP configuration updates failed", "3", "4"            };

    MPUInitCntr++;

    //Serial.print(F("MPU connection Try #"));
    //Serial.println(MPUInitCntr);
    //Serial.print(F("DMP Initialization failed (code "));
    //Serial.print(StatStr[devStatus]);
    //Serial.println(F(")"));

    if (MPUInitCntr >= 10) return; //only try 10 times
    delay(1000);
    MPU6050Connect(); // Lets try again
    return;
  }

  mpu.setXAccelOffset(MPUOffsets[0]);
  mpu.setYAccelOffset(MPUOffsets[1]);
  mpu.setZAccelOffset(MPUOffsets[2]);
  mpu.setXGyroOffset(MPUOffsets[3]);
  mpu.setYGyroOffset(MPUOffsets[4]);
  mpu.setZGyroOffset(MPUOffsets[5]);

  //Serial.println(F("Enabling DMP..."));
  mpu.setDMPEnabled(true);
  // enable Arduino interrupt detection

  //Serial.println(F("Enabling interrupt detection (Arduino external interrupt pin 2 on the Uno)..."));
  attachInterrupt(0, dmpDataReady, FALLING); //pin 2 on the Uno

  mpuIntStatus = mpu.getIntStatus(); // Same
  // get expected DMP packet size for later comparison
  packetSize = mpu.dmpGetFIFOPacketSize();
  delay(1000); // Let it Stabalize
  mpu.resetFIFO(); // Clear fifo buffer
  mpu.getIntStatus();
  mpuInterrupt = false; // wait for next interrupt

}
// ================================================================
// ===                      i2c SETUP Items                     ===
// ================================================================
void i2cSetup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
}

int getAngle()
{
  int temp=250;
  while (abs(temp)>180){
    if(mpuInterrupt){
      GetDMP();
      temp = Yaw;
    }
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
  //Serial.print("target angle ");
  //Serial.println(final);
  if (final+3 >180){
    ////Serial.println("  target near to 180 ");
    bool inRange = ((currentAngle > final - 3) || (currentAngle < final - 357));
    return inRange;
  }
  else if(final-3 < -180){
    bool inRange = ((currentAngle > final + 357) || (currentAngle < final + 3));
    ////Serial.print("Within the range: ");
    ////Serial.println(inRange);
    return inRange;
  }
  else{
    bool inRange = ((currentAngle > final - 3) && (currentAngle < final+3));
    return inRange;
  }
}

void turnAround(int turnAngle,int dir_turn)
{
  int initialAngle = getAngle();
  int temp;
  //Serial.print("Initial Angle: ");
  int count = 0;
  while(count<5){
    temp = getAngle();
    if(abs(temp-initialAngle)<2){
      count++;
    }
    else{
      count = 0;
    }
    initialAngle = temp;
  }
  //Serial.println(initialAngle);
  int currentAngle = initialAngle;
  //int targetAngle = getTarget(initialAngle);
  while (true)
  {
    dir = dir_turn;
    pwmL = 150;
    pwmR = 150;
    dl = 10;
    moveBot(dir,pwmL,pwmR);
    currentAngle=getAngle();
    //Serial.print("current angle ");
    //Serial.print(currentAngle);
    bool inRange =getTarget(initialAngle, currentAngle, turnAngle);
    //Serial.print("In range: ");
    //Serial.println(inRange);
    if(inRange == true)
    {
      Serial.println("0");
      break;
    }

  }
  moveBot(5, 0, 0);
  delay(10);
}

void setup() {
  Serial.begin(115200); //115200
  while (!Serial);

  i2cSetup();

  //Serial.println(F("Alive"));
  MPU6050Connect();
  pinMode(LED_PIN, OUTPUT); // LED Blinks when you are recieving FIFO packets from your MPU6050

}
void moveForward(int angle)
{
  if(angle < 0)
    angle*=3;
  else
    angle = int(2*angle);
  moveBot(1,100+angle,100-angle);
  Serial.println("0");
}
void stopBot()
{
  moveBot(5,0,0);
  Serial.println("0");
}

void loop() {
  char buffer[4];
  while(!Serial.available());
  Serial.readBytes( buffer, 3);
  int value = atoi(buffer);
  Serial.flush();
  int state = 5;
  state = value/100;
  int angle = value%100 - 25;
  Serial.print("TEST ");
  Serial.print(state);
  Serial.print("   Angle: ");
  Serial.println(angle);

  //Serial.println(state);

  switch(state)

  {
  case 1:
    moveForward(angle);
    break;
  case 2:
    turnAround(90,4);
    state=4;
    break;
  case 3:
    turnAround(-90,3);
    state=4;
    break;  
  case 4:
    stopBot();
    break;
  case 5:
    turnAround(180);
    state=4;
    break;  
  default:
    stopBot();

  }
}



void GetDMP() { // Best version I have made so far
  // //Serial.println(F("FIFO interrupt at:"));
  // //Serial.println(micros());
  mpuInterrupt = false;
  FifoAlive = 1;
  fifoCount = mpu.getFIFOCount();
  /*
  fifoCount is a 16-bit unsigned value. Indicates the number of bytes stored in the FIFO buffer.
   This number is in turn the number of bytes that can be read from the FIFO buffer and it is
   directly proportional to the number of samples available given the set of sensor data bound
   to be stored in the FIFO
   */

  // PacketSize = 42; refference in MPU6050_6Axis_MotionApps20.h Line 527
  // FIFO Buffer Size = 1024;
  uint16_t MaxPackets = 20;// 20*42=840 leaving us with  2 Packets (out of a total of 24 packets) left before we overflow.
  // If we overflow the entire FIFO buffer will be corrupt and we must discard it!

  // At this point in the code FIFO Packets should be at 1 99% of the time if not we need to look to see where we are skipping samples.
  if ((fifoCount % packetSize) || (fifoCount > (packetSize * MaxPackets)) || (fifoCount < packetSize)) { // we have failed Reset and wait till next time!
    digitalWrite(LED_PIN, LOW); // lets turn off the blinking light so we can see we are failing.
    //Serial.println(F("Reset FIFO"));
    if (fifoCount % packetSize) //Serial.print(F("\t Packet corruption")); // fifoCount / packetSize returns a remainder... Not good! This should never happen if all is well.
      //Serial.print(F("\tfifoCount ")); //Serial.print(fifoCount);
      //Serial.print(F("\tpacketSize ")); //Serial.print(packetSize);

      mpuIntStatus = mpu.getIntStatus(); // reads MPU6050_RA_INT_STATUS       0x3A
    //Serial.print(F("\tMPU Int Status ")); //Serial.print(mpuIntStatus , BIN);
    // MPU6050_RA_INT_STATUS       0x3A
    //
    // Bit7, Bit6, Bit5, Bit4          , Bit3       , Bit2, Bit1, Bit0
    // ----, ----, ----, FIFO_OFLOW_INT, I2C_MST_INT, ----, ----, DATA_RDY_INT

    /*
    Bit4 FIFO_OFLOW_INT: This bit automatically sets to 1 when a FIFO buffer overflow interrupt has been generated.
     Bit3 I2C_MST_INT: This bit automatically sets to 1 when an I2C Master interrupt has been generated. For a list of I2C Master interrupts, please refer to Register 54.
     Bit1 DATA_RDY_INT This bit automatically sets to 1 when a Data Ready interrupt is generated.
     */
    if (mpuIntStatus & B10000) { //FIFO_OFLOW_INT
      //Serial.print(F("\tFIFO buffer overflow interrupt "));
    }
    if (mpuIntStatus & B1000) { //I2C_MST_INT
      //Serial.print(F("\tSlave I2c Device Status Int "));
    }
    if (mpuIntStatus & B1) { //DATA_RDY_INT
      //Serial.print(F("\tData Ready interrupt "));
    }
    //Serial.println();
    //I2C_MST_STATUS
    //PASS_THROUGH, I2C_SLV4_DONE,I2C_LOST_ARB,I2C_SLV4_NACK,I2C_SLV3_NACK,I2C_SLV2_NACK,I2C_SLV1_NACK,I2C_SLV0_NACK,
    mpu.resetFIFO();// clear the buffer and start over
    mpu.getIntStatus(); // make sure status is cleared we will read it again.
  } 
  else {
    while (fifoCount  >= packetSize) { // Get the packets until we have the latest!
      if (fifoCount < packetSize) break; // Something is left over and we don't want it!!!
      mpu.getFIFOBytes(fifoBuffer, packetSize); // lets do the magic and get the data
      fifoCount -= packetSize;
    }
    MPUMath(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<< On success MPUMath() <<<<<<<<<<<<<<<<<<<
    digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Blink the Light
    if (fifoCount > 0) mpu.resetFIFO(); // clean up any leftovers Should never happen! but lets start fresh if we need to. this should never happen.
  }
}


void MPUMath() {
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  Yaw = (ypr[0] * 180 / M_PI);
  Pitch = (ypr[1] * 180 / M_PI);
  Roll = (ypr[2] * 180 / M_PI);
}




