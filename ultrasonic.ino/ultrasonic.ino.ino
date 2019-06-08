//void setup(){
//  Serial.begin(9600);
//}
//
//void loop(){
//  Serial.println("Hello world");
//  Serial.flush();
//  delay(1000);
//}


const int pingPin = 6; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 7; // Echo Pin of Ultrasonic Sensor

void setup() {
   Serial.begin(9600); // Starting Serial Terminal
   pinMode(pingPin, OUTPUT);
   pinMode(echoPin, INPUT);
}

void loop() {
   double duration, inches, cm;
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration);
   Serial.print("obstacle ");
   Serial.print(cm);
   Serial.println();
   Serial.flush();
   delay(1);
}

double microsecondsToCentimeters(long microseconds) {
   return (double)microseconds / 29 / 2;
}
