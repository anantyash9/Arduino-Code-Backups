#define magSensorL 31
#define magSensorR 33
#define ledPin 13


#define lMotor1 24
#define lMotor2 25
#define lMotorPWM 8

#define rMotor1 22
#define rMotor2 23
#define rMotorPWM 9

int dir, pwmL, pwmR, dl;

void setup() {
  pinMode(rMotor1, OUTPUT);
  pinMode(rMotor2, OUTPUT);
  pinMode(rMotorPWM, OUTPUT);

  pinMode(lMotor1, OUTPUT);
  pinMode(lMotor2, OUTPUT);
  pinMode(lMotorPWM, OUTPUT);  

  
  pinMode(magSensorL, INPUT);
  pinMode(magSensorR, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  // put your setup code here, to run once:

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

bool left=false;
bool right=false;
int intensity=1;
void loop() {
  int valL = digitalRead(magSensorL);
  int valR = digitalRead(magSensorR);
  if(valL == LOW){
    if(left==true)
      intensity=intensity+1;
    else
      intensity=10;  
    left=true;
    right=false;
  }
  else if(valR == LOW){
    if(right==true)
      intensity=intensity+10;
    else
      intensity=1;
    right=true;
    left=false;
  }  // put your main code here, to run repeatedly:

  dir = 1;
  pwmL = 70;
  pwmR = 70;
  dl=1;

  if (left)
    {
      dir = 1;
      pwmL = 70-min(30,intensity);
      pwmR = 70+min(10,intensity);
      dl=1;
      Serial.println("Lefting "+String(intensity));
      }
  
  if (right)
    {
      dir = 1;
      pwmL = 70+min(30,intensity);
      pwmR = 70-min(10,intensity);
      dl=1;
      Serial.println("Righting "+String(intensity));
      }
  moveBot(dir, pwmL, pwmR);
  delay(dl);   
}
