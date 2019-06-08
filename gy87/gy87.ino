#include "MPU6050.h"
#include "QMC5883L.h"

QMC5883L compass;
MPU6050 accelgyro;

const int pingPin = 6; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 7; // Echo Pin of Ultrasonic Sensor

void setup()
{
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  compass.init();
  accelgyro.initialize();
  accelgyro.setI2CBypassEnabled(true);
  Serial.begin(9600);
}


void loop()
{
  //  int r;
  //  int16_t x, y, z, t;
  //  r = compass.readRaw(&x, &y, &z, &t);
  //  //Serial.println(String(x) + " " + String(y) + " " + String(z)+ " "+ String(t));


  double duration, inches, cm;
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);

  Serial.println("heading " +String(compass.readHeading())+ " obstacle " + String(cm));
  Serial.flush();

  delay(100);

}

double microsecondsToCentimeters(long microseconds) {
   return (double)microseconds / 29 / 2;
}
