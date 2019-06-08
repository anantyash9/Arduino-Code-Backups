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
#define sensorOut4 11


#define lMotor1 22
#define lMotor2 23
#define lMotorPWM 8

#define rMotor1 24
#define rMotor2 25
#define rMotorPWM 9

int frequencyLr = 0;
int frequencyLg = 0;
int frequencyLb = 0;

int frequencyMr = 0;
int frequencyMg = 0;
int frequencyMb = 0;

int frequencyRr = 0;
int frequencyRg = 0;
int frequencyRb = 0;

int effect= 0;

float *hsvL = new float[3];
float *hsvM = new float[3];
float *hsvR = new float[3];

int dir, pwmL, pwmR;

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
  frequencyMr = pulseIn(sensorOut2, LOW);
  frequencyRr = pulseIn(sensorOut3, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  if(useMapping){
    frequencyLr = map(frequencyLr, 149,341,255,0);
    frequencyMr = map(frequencyMr, 225,440,255,0);
    frequencyRr = map(frequencyRr, 160,368,255,0);
  }
  if(printSensor1){
  // Printing the value on the serial monitor
  Serial.print("R1 ");//printing name
  Serial.print(frequencyLr);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor2){
  // Printing the value on the serial monitor
  Serial.print("R2 ");//printing name
  Serial.print(frequencyMr);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor3){
  // Printing the value on the serial monitor
  Serial.print("R3 ");//printing name
  Serial.print(frequencyRr);//printing RED color frequency
  Serial.print(" ");
  }  
  delay(10);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequencyLg = pulseIn(sensorOut1, LOW);
  frequencyMg = pulseIn(sensorOut2, LOW);
  frequencyRg = pulseIn(sensorOut3, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  if(useMapping){
    frequencyLg = map(frequencyLg, 154,344,255,0);
    frequencyMg = map(frequencyMg, 210,424,255,0);
    frequencyRg = map(frequencyRg, 172,367,255,0);
  }
  if(printSensor1){
  // Printing the value on the serial monitor
  Serial.print("G1 ");//printing name
  Serial.print(frequencyLg);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor2){
  // Printing the value on the serial monitor
  Serial.print("G2 ");//printing name
  Serial.print(frequencyMg);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor3){
  // Printing the value on the serial monitor
  Serial.print("G3 ");//printing name
  Serial.print(frequencyRg);//printing RED color frequency
  Serial.print(" ");
  } 
  delay(10);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequencyLb = pulseIn(sensorOut1, LOW);
  frequencyMb = pulseIn(sensorOut2, LOW);
  frequencyRb = pulseIn(sensorOut3, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  if(useMapping){
    frequencyLb = map(frequencyLb, 112,251,255,0);
    frequencyMb = map(frequencyMb, 209,423,255,0);
    frequencyRb = map(frequencyRb, 173,374,255,0);
  }
  if(printSensor1){
  // Printing the value on the serial monitor
  Serial.print("B1 ");//printing name
  Serial.print(frequencyLb);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor2){
  // Printing the value on the serial monitor
  Serial.print("B2 ");//printing name
  Serial.print(frequencyMb);//printing RED color frequency
  Serial.print(" ");
  }
  if(printSensor3){
  // Printing the value on the serial monitor
  Serial.print("B3 ");//printing name
  Serial.print(frequencyRb);//printing RED color frequency
  Serial.print(" ");
  } 
  delay(10);

  rgb2hsv(frequencyLr, frequencyLg, frequencyLb, hsvL);
  rgb2hsv(frequencyMr, frequencyMg, frequencyMb, hsvM);
  rgb2hsv(frequencyRr, frequencyRg, frequencyRb, hsvR);
  //Serial.println(String(hsvL[0])+", "+String(hsvM[0])+", "+String(hsvR[0]));
  dir = 1;
  pwmL = 90;
  pwmR = 90;

  if((hsvL[0] > 0.3 and hsvL[0] < 0.80)and(hsvM[0] > 0.3 and hsvM[0] < 0.80)and(hsvR[0] > 0.3 and hsvR[0] < 0.80)){
    dir = 5;
    Serial.println("I have no purpose in life " );
  }
  
   if((hsvL[0] < 0.3 or hsvL[0] > 0.75)and(hsvM[0] < 0.3 or hsvM[0] > 0.75)){
    dir = 5;
    pwmL = 90+120;
    pwmR = 90-80;
    Serial.println("Turn Right like crazy ");
  }
  
  else if((hsvR[0] < 0.3 or hsvR[0] > 0.75)and(hsvM[0] < 0.3 or hsvM[0] > 0.75)){
    dir = 5;
    pwmL = 90-80;
    pwmR = 90+120;
    Serial.println("Turn Left like crazy ");
  }
  else if(hsvL[0] < 0.3 or hsvL[0] > 0.75){
    if (hsvL[0] < 0.3)
    hsvL[0]=1;
    effect= (hsvL[0] - 0.75)*300;
    dir = 5;
    pwmL = 90+effect;
    pwmR = 90-effect;
    Serial.println("Move Right " + String(effect));
  }
  else if(hsvR[0] < 0.3 or hsvR[0] > 0.75){
    if (hsvR[0] < 0.3)
    hsvR[0]=1;
    effect= (hsvR[0] - 0.75)*300;
    dir = 5;
    pwmL = 90-effect;
    pwmR = 90+effect;
    Serial.println("Move Left "+ String(effect));
  }
 

  moveBot(dir, pwmL, pwmR);
  
}
