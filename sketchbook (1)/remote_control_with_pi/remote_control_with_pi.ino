
#define lMotor1 24
#define lMotor2 25
#define lMotorPWM 8

#define rMotor1 22
#define rMotor2 23
#define rMotorPWM 9
int pwmL = 0;
int pwmR = 0;
int n = 5;
void setup() {
  // put your setup code here, to run once:
  pinMode(rMotor1, OUTPUT);
  pinMode(rMotor2, OUTPUT);
  pinMode(rMotorPWM, OUTPUT);

  pinMode(lMotor1, OUTPUT);
  pinMode(lMotor2, OUTPUT);
  pinMode(lMotorPWM, OUTPUT); 
  
  Serial.begin(9600);

}

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

void loop() {
  if(Serial.available()){
    n = Serial.read() - '0';
  }
  moveBot(n, 255, 255);
  delay(100);
}
