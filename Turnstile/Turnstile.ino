int fPin = 7;
int rPin = 8;

void setup() {
  Serial.begin(9600); 
  pinMode(fPin, OUTPUT);
  pinMode(rPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0){
    switch((char)Serial.read()){
      case '0':
        Serial.println("fPin");
        digitalWrite(fPin, HIGH);
        break;
      case '1':
        Serial.println("rPin");
        digitalWrite(rPin, HIGH);
        break;
      default:
        Serial.println("-");
    }
  }
  delay(250);  
  reset();  
}

void reset(){
  digitalWrite(fPin, LOW);
  digitalWrite(rPin, LOW);
}
