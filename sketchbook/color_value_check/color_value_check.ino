/*     Arduino Color Sensing Tutorial
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
 
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

int frequencyRed[7] = {0};
int frequencyBlue[7] = {0};
int frequencyGreen[7] = {0};

int effect= 0;

float *hsvM = new float[3];
float *hsvR = new float[3];
float *hsvER = new float[3];
float *hsvEER = new float[3];
float *hsvL = new float[3];
float *hsvEL = new float[3];
float *hsvEEL = new float[3];

int dir, pwmL, pwmR, dl;

void setup() {
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
  
  Serial.begin(115200);
}

boolean useMapping = true;
boolean printSensor1 = false;
boolean printSensor2 = false;
boolean printSensor3 = false;
boolean printSensor4 = false;
boolean printSensor5 = false;
boolean printSensor6 = false;
boolean printSensor7 = false;

//Move Robot
void moveBot(int dir, int Lpwm, int Rpwm){
  analogWrite(lMotorPWM, Lpwm);
  analogWrite(rMotorPWM, Rpwm);
  delay(10);

  switch(dir){
    case 1: digitalWrite(lMotor1, HIGH);
            digitalWrite(lMotor2, LOW);
            digitalWrite(rMotor1, HIGH);
            digitalWrite(rMotor2, LOW);
            break;
            
    case 2: digitalWrite(lMotor1, LOW);
            digitalWrite(lMotor2, HIGH);
            digitalWrite(rMotor1, LOW);
            digitalWrite(rMotor2, HIGH);
            break;

    case 3: digitalWrite(lMotor1, LOW);
            digitalWrite(lMotor2, HIGH);
            digitalWrite(rMotor1, HIGH);
            digitalWrite(rMotor2, LOW);
            break;

    case 4: digitalWrite(lMotor1, HIGH);
            digitalWrite(lMotor2, LOW);
            digitalWrite(rMotor1, LOW);
            digitalWrite(rMotor2, HIGH);
            break;

    default:digitalWrite(lMotor1, LOW);
            digitalWrite(lMotor2, LOW);
            digitalWrite(rMotor1, LOW);
            digitalWrite(rMotor2, LOW);
            analogWrite(lMotorPWM, 0);
            analogWrite(rMotorPWM, 0);
  }
}

//RGB2HSV conversion and helper functions
float fract(float x) { return x - int(x); }

float mix(float a, float b, float t) { return a + (b - a) * t; }

float step(float e, float x) { return x < e ? 0.0 : 1.0; }

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
void loop() {
  double cm = readUltraSonic();
  
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
  Serial.print("EER: ");
  Serial.print(hsvEER[0]);
  Serial.print(", ");
  Serial.print(hsvEER[2]);
  Serial.print(", ER: ");
  Serial.print(hsvER[0]);
  Serial.print(", ");
  Serial.print(hsvER[2]);
  Serial.print(", R: ");
  Serial.print(hsvR[0]);
  Serial.print(", ");
  Serial.print(hsvR[2]);
  Serial.print(", M: ");
  Serial.print(hsvM[0]);
  Serial.print(", ");
  Serial.print(hsvM[2]);
  Serial.print(", L: ");
  Serial.print(hsvL[0]);
  Serial.print(", ");
  Serial.print(hsvL[2]);
  Serial.print(", EL: ");
  Serial.print(hsvEL[0]);
  Serial.print(", ");
  Serial.print(hsvEL[2]);
  Serial.print(", EEL: ");
  Serial.println(hsvEEL[0]);
  Serial.print(", ");
  Serial.println(hsvEEL[2]);
  

  moveBot(5, 0, 0);
  delay(dl);
}
