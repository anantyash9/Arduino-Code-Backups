
int motor11 = 22;
int motor12 = 23;
int motor21 = 24;
int motor22 = 25;
int en1 = 2;
int en2 = 3;
int controlPinF = 48;
int controlPinB = 49;
int controlPinL = 50;
int controlPinR = 51;
int controlIncS = 52;
int controlDecS = 53;
int stopPin = 46;
int val = 0;
int dir = 5;
int fwd = LOW;
int bd = LOW;
int left = LOW;
int right  = LOW;
int stopKey = LOW;
int incSpeed = LOW;
int decSpeed = LOW;

void setup() {
  pinMode(motor11, OUTPUT);
  pinMode(motor12, OUTPUT);
  pinMode(motor21, OUTPUT);
  pinMode(motor22, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(controlPinF, INPUT);
  pinMode(controlPinB, INPUT);
  pinMode(controlPinL, INPUT);
  pinMode(controlPinR, INPUT);
  pinMode(controlIncS, INPUT);
  pinMode(controlDecS, INPUT);
  pinMode(stopPin, INPUT);
  Serial.begin(9600);
}

void moveBot(int dirn, int value){
  int dly = 350;
  analogWrite(en1, value);
  analogWrite(en2, value);
  switch(dirn){
    case 1: digitalWrite(motor11, HIGH);
            digitalWrite(motor12, LOW);
            digitalWrite(motor21, HIGH);
            digitalWrite(motor22, LOW);
            delay(dly);
            break;
    case 2: digitalWrite(motor11, LOW);
            digitalWrite(motor12, HIGH);
            digitalWrite(motor21, LOW);
            digitalWrite(motor22, HIGH);
            delay(dly);
            break;
    case 3: digitalWrite(motor11, HIGH);
            digitalWrite(motor12, LOW);
            digitalWrite(motor21, LOW);
            digitalWrite(motor22, HIGH);
            delay(dly);
            break;
    case 4: digitalWrite(motor11, LOW);
            digitalWrite(motor12, HIGH);
            digitalWrite(motor21, HIGH);
            digitalWrite(motor22, LOW);
            delay(dly);
            break;
    default:digitalWrite(motor11, LOW);
            digitalWrite(motor12, LOW);
            digitalWrite(motor21, LOW);
            digitalWrite(motor22, LOW);
            delay(dly);
  }
}
void loop() {
  fwd = digitalRead(controlPinF);
  bd = digitalRead(controlPinB);
  left = digitalRead(controlPinL);
  right = digitalRead(controlPinR);
  stopKey = digitalRead(stopPin);

  incSpeed = digitalRead(controlIncS);
  decSpeed = digitalRead(controlDecS);

  //Choose direction of movement
  if(fwd == HIGH){
    dir = 1;
  }
  else if(bd == HIGH){
    dir = 2;
  }
  else if(left == HIGH){
    dir = 3;
  }
  else if(right == HIGH){
    dir = 4;
  }
  // Enables Stop Key
  if(stopKey == HIGH){
    dir = 5;
  }

  //Change the speed
  if(incSpeed == HIGH){
    val = 100;
  }
  if(decSpeed == HIGH){
    val = 1000;
  }
  Serial.print(dir);
  Serial.print(", ");
  Serial.println(val);

  val = 70;
  moveBot(dir, val);
}
