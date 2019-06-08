int motor11 = 22;
int motor12 = 23;
int motor21 = 24;
int motor22 = 25;
int en1 = 2;
int en2 = 3;
void setup() {
  pinMode(motor11, OUTPUT);
  pinMode(motor12, OUTPUT);
  pinMode(motor21, OUTPUT);
  pinMode(motor22, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);

}

void moveBot(int dirn, int value){
  int dly = 50;
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
  // put your main code here, to run repeatedly:
  int val = 70;
  moveBot(2, val);
}
