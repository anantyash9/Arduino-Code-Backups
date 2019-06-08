/*     Arduino Color Sensing Tutorial
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
 
#define S0 50           //previous - 4
#define S1 51           //previous - 5
#define S2 48           //previous - 9
#define S3 49           //previous - 10
#define sensorOut1 45   //previous - 8
#define sensorOut2 46   //previous - 9
#define sensorOut3 47   //previous - 10
#define sensorOut4 44


#define lMotor1 24
#define lMotor2 25
#define lMotorPWM 8

#define rMotor1 22
#define rMotor2 23
#define rMotorPWM 9

int frequencyLr = 0;
int frequencyLg = 0;
int frequencyLb = 0;

int frequencyERr = 0;
int frequencyERg = 0;
int frequencyERb = 0;

int frequencyELr = 0;
int frequencyELg = 0;
int frequencyELb = 0;

int frequencyRr = 0;
int frequencyRg = 0;
int frequencyRb = 0;

int effect= 0;

float *hsvnR = new float[3];
float *hsvER = new float[3];
float *hsvnL = new float[3];
float *hsvEL = new float[3];

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

boolean useMapping = true;
boolean printSensor1 = false;
boolean printSensor2 = false;
boolean printSensor3 = false;
boolean printSensor4 = false;


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

void loop() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequencyLr = pulseIn(sensorOut1, LOW);
  frequencyERr = pulseIn(sensorOut2, LOW);
  frequencyRr = pulseIn(sensorOut3, LOW);
  frequencyELr = pulseIn(sensorOut4, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  if(useMapping){
    frequencyLr = map(frequencyLr, 200,410,255,0);
    frequencyERr = map(frequencyERr, 198,487,255,0);
    frequencyRr = map(frequencyRr, 176,346,255,0);
    frequencyELr = map(frequencyELr, 222,531,255,0);
  }
  if(printSensor1){
  // Printing the value on the serial monitor
  Serial.print("Lr ");//printing name
  Serial.print(frequencyLr);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor2){
  // Printing the value on the serial monitor
  Serial.print("ERr ");//printing name
  Serial.print(frequencyERr);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor3){
  // Printing the value on the serial monitor
  Serial.print("Rr ");//printing name
  Serial.print(frequencyRr);//printing RED color frequency
  Serial.print(" ");
  }
    if(printSensor4){
  // Printing the value on the serial monitor
  Serial.print("ELr ");//printing name
  Serial.print(frequencyELr);//printing RED color frequency
  Serial.print(" ");
  }  
  delay(10);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequencyLg = pulseIn(sensorOut1, LOW);
  frequencyERg = pulseIn(sensorOut2, LOW);
  frequencyRg = pulseIn(sensorOut3, LOW);
  frequencyELg = pulseIn(sensorOut4, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  if(useMapping){
    frequencyLg = map(frequencyLg, 191,406,255,0);
    frequencyERg = map(frequencyERg, 198,489,255,0);
    frequencyRg = map(frequencyRg, 176,353,255,0);
    frequencyELg = map(frequencyELg, 217,527,255,0);
  }
  if(printSensor1){
  // Printing the value on the serial monitor
  Serial.print("Lg ");//printing name
  Serial.print(frequencyLg);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor2){
  // Printing the value on the serial monitor
  Serial.print("ERg ");//printing name
  Serial.print(frequencyERr);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor3){
  // Printing the value on the serial monitor
  Serial.print("Rg ");//printing name
  Serial.print(frequencyRg);//printing RED color frequency
  Serial.print(" ");
  }
    if(printSensor4){
  // Printing the value on the serial monitor
  Serial.print("ELg ");//printing name
  Serial.print(frequencyELg);//printing RED color frequency
  Serial.print(" ");
  } 
  delay(10);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequencyLb = pulseIn(sensorOut1, LOW);
  frequencyERb = pulseIn(sensorOut2, LOW);
  frequencyRb = pulseIn(sensorOut3, LOW);
  frequencyELb = pulseIn(sensorOut4, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  if(useMapping){
    frequencyLb = map(frequencyLb, 142,301,255,0);
    frequencyERb = map(frequencyERb, 149,366,255,0);
    frequencyRb = map(frequencyRb, 130,259,286,0);
    frequencyELb = map(frequencyELb, 169,420,255,0);
  }
  if(printSensor1){
  // Printing the value on the serial monitor
  Serial.print("Lb ");//printing name
  Serial.print(frequencyLb);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor2){
  // Printing the value on the serial monitor
  Serial.print("ERb ");//printing name
  Serial.print(frequencyERb);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor3){
  // Printing the value on the serial monitor
  Serial.print("Rb ");//printing name
  Serial.print(frequencyRb);//printing RED color frequency
  Serial.print(" ");
  }
    if(printSensor4){
  // Printing the value on the serial monitor
  Serial.print("ELb ");//printing name
  Serial.print(frequencyELb);//printing RED color frequency
  Serial.print(" ");
  } 
  delay(10);

  rgb2hsv(frequencyLr, frequencyLg, frequencyLb, hsvnR);
  rgb2hsv(frequencyERr, frequencyERg, frequencyERb, hsvER);
  rgb2hsv(frequencyRr, frequencyRg, frequencyRb, hsvnL);
  rgb2hsv(frequencyELr, frequencyELg, frequencyELb, hsvEL);
  Serial.println(String(hsvEL[0])+","+String(hsvnL[0])+", "+String(hsvnR[0])+", "+String(hsvER[0]));
  dir = 1;
  pwmL = 60;
  pwmR = 60;
  dl=1;

//  if((hsvnR[0] > 0.3 and hsvnR[0] < 0.80)and(hsvM[0] > 0.3 and hsvM[0] < 0.80)and(hsvnL[0] > 0.3 and hsvnL[0] < 0.80)){
//    dir = 5;
//    Serial.println("I have no purpose in life " );
//  }
  
   if((hsvER[0] < 0.3)){
    dir = 4;
//    dir = 5;
    pwmL = 60+120;
    pwmR = 50;
    dl = 500;
    Serial.println("Turn Right like crazy ");
  }
  
  else if((hsvEL[0] < 0.3)){
    dir = 3;
//    dir = 5;
    pwmL = 50;
    pwmR = 60+120;
    dl = 500;
    Serial.println("Turn Left like crazy ");
  }
  else if(hsvnL[0] < 0.3 or hsvnL[0] > 0.7){
    if (hsvnL[0] < 0.3)
    hsvnL[0]=1;
    effect= min(40,(hsvnL[0] - 0.60)*350);
    dir = 1;
    pwmR = 60+effect;
    pwmL = max(40,60-effect);
    dl = 1;
    Serial.println("Move Right " + String(effect));
  }
  else if(hsvnR[0] < 0.3 or hsvnR[0] > 0.60){
    if (hsvnR[0] < 0.3)
    hsvnR[0]=1;
    effect= min(40,(hsvnR[0] - 0.60)*350);
    dir = 1;
    pwmR = max(40,60-effect);
    pwmL = 60+effect;
    dl = 1;
    Serial.println("Move Left "+ String(effect));
  }
 

  moveBot(dir, pwmL, pwmR);
  delay(dl);
  
}
