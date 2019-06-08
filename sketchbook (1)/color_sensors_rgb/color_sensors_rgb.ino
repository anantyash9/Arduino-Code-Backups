#define S0 50           //previous - 4
#define S1 51           //previous - 5
#define S2 48           //previous - 9
#define S3 49           //previous - 10
#define sensorOut1 41   //previous - 8
#define sensorOut2 42   //previous - 9
#define sensorOut3 43   //previous - 10
#define sensorOut4 44
#define sensorOut5 45
#define sensorOut6 46
#define sensorOut7 47

//int frequencyRed[7] = {0};
//int frequencyBlue[7] = {0};
//int frequencyGreen[7] = {0};

int lineSensorRGB[3][7] = {
  0};
float lineSensorHSV[3][7] = {
  0};

int sensorOut[7] = {
  41,42,43,44,45,46,47};


void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut[0], INPUT);
  pinMode(sensorOut[1], INPUT);
  pinMode(sensorOut[2], INPUT);
  pinMode(sensorOut[3], INPUT);
  pinMode(sensorOut[4], INPUT);
  pinMode(sensorOut[5], INPUT);
  pinMode(sensorOut[6], INPUT);

  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  Serial.begin(115200);
}

boolean useMapping = true;

void loop(){
  updateLineRed();
  updateLineGreen();
  updateLineBlue();  
  updateLineHSV();
  
  printSensor(lineSensorHSV[2]);


  Serial.println();
}

void updateLineRed(){
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);

  // Reading the output frequency
  readSensor(lineSensorRGB[0]);
  if(useMapping)
    mapRange(lineSensorRGB[0], 23000);
}

void updateLineGreen(){
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  readSensor(lineSensorRGB[1]);
  if(useMapping)
    mapRange(lineSensorRGB[1], 23000);
}

void updateLineBlue(){
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  readSensor(lineSensorRGB[2]);
  if(useMapping)
    mapRange(lineSensorRGB[2], 19000);
}

void updateLineHSV(){
  for(int i=0;i<7;i++){
    float *hsv = new float[3];    
    rgb2hsv(lineSensorRGB[0][i],lineSensorRGB[1][i],lineSensorRGB[2][i], hsv);
    lineSensorHSV[0][i]= hsv[0];
    lineSensorHSV[1][i]= hsv[1];
    lineSensorHSV[2][i]= hsv[2];
  }
}

void mapRange(int color[], int maximum){
  for(int i=0;i<7;i++){
    color[i] = map(color[i], 0,maximum,255,0);
  }
}

void readSensor(int color[]){
  for(int i=0;i<7;i++){
    color[i] = pulseIn(sensorOut[i], LOW);
  }
}

void printSensor(int color[]){
  for(int i= 6; i >=0; i--){
    Serial.print(color[i]);
    Serial.print(" ");
  }
}
void printSensor(float color[]){
  for(int i= 6; i >=0; i--){
    Serial.print(color[i]);
    Serial.print(" ");
  }
}

//RGB2HSV conversion and helper functions
float fract(float x) { 
  return x - int(x); 
}

float mix(float a, float b, float t) { 
  return a + (b - a) * t; 
}

float step(float e, float x) { 
  return x < e ? 0.0 : 1.0; 
}

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





