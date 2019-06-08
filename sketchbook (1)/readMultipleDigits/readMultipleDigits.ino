void setup(){
  Serial.begin(115200);
}

void loop(){
  char buffer[4];
  while(!Serial.available());
  Serial.readBytes( buffer, 3);
  int value = atoi(buffer);
  Serial.println(value);
}
