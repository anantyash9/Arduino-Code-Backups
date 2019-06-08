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


#define lMotor1 24
#define lMotor2 25
#define lMotorPWM 8

#define rMotor1 22
#define rMotor2 23
#define rMotorPWM 9

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
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  Serial.begin(115200);
}

boolean useMapping = false;
boolean printSensor1 = true;
boolean printSensor2 = true;
boolean printSensor3 = true;
boolean printSensor4 = true;
boolean printSensor5 = true;
boolean printSensor6 = true;
boolean printSensor7 = true;

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


//Main function
void loop() {
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
    frequencyRed[0] = map(frequencyRed[0], 200,410,255,0);
    frequencyRed[1] = map(frequencyRed[1], 198,487,255,0);
    frequencyRed[2] = map(frequencyRed[2], 176,346,255,0);
    frequencyRed[3] = map(frequencyRed[3], 222,531,255,0);
    frequencyRed[4] = map(frequencyRed[4], 200,410,255,0);
    frequencyRed[5] = map(frequencyRed[5], 198,487,255,0);
    frequencyRed[6] = map(frequencyRed[6], 176,346,255,0);
  }
  delay(10);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
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
    frequencyBlue[0] = map(frequencyBlue[0], 200,410,255,0);
    frequencyBlue[1] = map(frequencyBlue[1], 198,487,255,0);
    frequencyBlue[2] = map(frequencyBlue[2], 176,346,255,0);
    frequencyBlue[3] = map(frequencyBlue[3], 222,531,255,0);
    frequencyBlue[4] = map(frequencyBlue[4], 200,410,255,0);
    frequencyBlue[5] = map(frequencyBlue[5], 198,487,255,0);
    frequencyBlue[6] = map(frequencyBlue[6], 176,346,255,0);
  }
   
  delay(10);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
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
    frequencyGreen[0] = map(frequencyGreen[0], 200,410,255,0);
    frequencyGreen[1] = map(frequencyGreen[1], 198,487,255,0);
    frequencyGreen[2] = map(frequencyGreen[2], 176,346,255,0);
    frequencyGreen[3] = map(frequencyGreen[3], 222,531,255,0);
    frequencyGreen[4] = map(frequencyGreen[4], 200,410,255,0);
    frequencyGreen[5] = map(frequencyGreen[5], 198,487,255,0);
    frequencyGreen[6] = map(frequencyGreen[6], 176,346,255,0);
  }
  if(printSensor1){
  // Printing the value on the serial monitor
  Serial.print("EER: ");//printing name
  Serial.print(frequencyRed[0]);
  Serial.print(", ");
  Serial.print(frequencyGreen[0]);
  Serial.print(", ");
  Serial.print(frequencyBlue[0]);
  Serial.print(" ");
  }
  if(printSensor2){
  // Printing the value on the serial monitor
  Serial.print("ER: ");//printing name
  Serial.print(frequencyRed[1]);
  Serial.print(", ");
  Serial.print(frequencyGreen[1]);
  Serial.print(", ");
  Serial.print(frequencyBlue[1]);
  Serial.print(" ");
  }
  if(printSensor3){
  // Printing the value on the serial monitor
  Serial.print("R: ");//printing name
  Serial.print(frequencyRed[2]);//printing RED color frequency
  Serial.print(", ");
  Serial.print(frequencyGreen[2]);
  Serial.print(", ");
  Serial.print(frequencyBlue[2]);
  Serial.print(" ");
  }
  if(printSensor4){
  // Printing the value on the serial monitor
  Serial.print("M: ");//printing name
  Serial.print(frequencyRed[3]);//printing RED color frequency
  Serial.print(", ");
  Serial.print(frequencyGreen[3]);
  Serial.print(", ");
  Serial.print(frequencyBlue[3]);
  Serial.print(" ");
  } 
  if(printSensor5){
  // Printing the value on the serial monitor
  Serial.print("L: ");//printing name
  Serial.print(frequencyRed[4]);
  Serial.print(", ");
  Serial.print(frequencyGreen[4]);
  Serial.print(", ");
  Serial.print(frequencyBlue[4]);
  Serial.print(" ");
  }
  if(printSensor6){
  // Printing the value on the serial monitor
  Serial.print("EL: ");//printing name
  Serial.print(frequencyRed[5]);//printing RED color frequency
  Serial.print(", ");
  Serial.print(frequencyGreen[5]);
  Serial.print(", ");
  Serial.print(frequencyBlue[5]);
  Serial.print(" ");
  }
  if(printSensor7){
  // Printing the value on the serial monitor
  Serial.print("EEL: ");//printing name
  Serial.print(frequencyRed[6]);//printing RED color frequency
  Serial.print(", ");
  Serial.print(frequencyGreen[6]);
  Serial.print(", ");
  Serial.print(frequencyBlue[6]);
  Serial.print(" ");
  } 
  delay(10);
//
//  rgb2hsv(frequencyLr, frequencyLg, frequencyLb, hsvnR);
//  rgb2hsv(frequencyERr, frequencyERg, frequencyERb, hsvER);
//  rgb2hsv(frequencyRr, frequencyRg, frequencyRb, hsvnL);
//  rgb2hsv(frequencyELr, frequencyELg, frequencyELb, hsvEL);
////  Serial.println(String(hsvEL[0])+","+String(hsvnL[0])+", "+String(hsvnR[0])+", "+String(hsvER[0]));
//  dir = 5;
//  pwmL = 60;
//  pwmR = 60;
//  dl=1;
//
////  if((hsvnR[0] > 0.3 and hsvnR[0] < 0.80)and(hsvM[0] > 0.3 and hsvM[0] < 0.80)and(hsvnL[0] > 0.3 and hsvnL[0] < 0.80)){
////    dir = 5;
////    Serial.println("I have no purpose in life " );
////  }
//  
//   if((hsvER[0] < 0.3)){
//    dir = 5;
////    dir = 5;
//    pwmL = 60+120;
//    pwmR = 50;
//    dl = 500;
//    Serial.println("Turn Right like crazy ");
//  }
//  
//  else if((hsvEL[0] < 0.3)){
//    dir = 5;
////    dir = 5;
//    pwmL = 50;
//    pwmR = 60+120;
//    dl = 500;
//    Serial.println("Turn Left like crazy ");
//  }
//  else if(hsvnL[0] < 0.3 or hsvnL[0] > 0.7){
//    if (hsvnL[0] < 0.3)
//    hsvnL[0]=1;
//    effect= min(40,(hsvnL[0] - 0.60)*350);
//    dir = 5;
//    pwmR = 60+effect;
//    pwmL = max(40,60-effect);
//    dl = 1;
//    Serial.println("Move Right " + String(effect));
//  }
//  else if(hsvnR[0] < 0.3 or hsvnR[0] > 0.60){
//    if (hsvnR[0] < 0.3)
//    hsvnR[0]=1;
//    effect= min(40,(hsvnR[0] - 0.60)*350);
//    dir = 5;
//    pwmR = max(40,60-effect);
//    pwmL = 60+effect;
//    dl = 1;
//    Serial.println("Move Left "+ String(effect));
//  }
// 
//
//  moveBot(dir, pwmL, pwmR);
//  delay(dl);
  
}
