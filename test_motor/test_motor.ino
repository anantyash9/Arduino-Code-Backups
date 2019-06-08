
#define lMotor1 24
#define lMotor2 25
#define lMotorPWM 8

#define rMotor1 22
#define rMotor2 23
#define rMotorPWM 9
 int pwm = 70;
 int change = 5;
void setup() {
  // put your setup code here, to run once:
  pinMode(rMotor1, OUTPUT);
  pinMode(rMotor2, OUTPUT);
  pinMode(rMotorPWM, OUTPUT);

  pinMode(lMotor1, OUTPUT);
  pinMode(lMotor2, OUTPUT);
  pinMode(lMotorPWM, OUTPUT); 

}

void loop() {
  // put your main code here, to run repeatedly:
//  if(pwm < 70 || pwm > 240){
//    change = -change;
//  }
//  pwm += change;
  digitalWrite(lMotor1, LOW);
  digitalWrite(lMotor2, HIGH);
  digitalWrite(rMotor1, HIGH);
  digitalWrite(rMotor2, LOW);
  analogWrite(lMotorPWM, 0);
  analogWrite(rMotorPWM, 128);
  delay(1000);

}
