//      *****Final code of Finflow for Techevence 9.0*****
//  Arduino Mega 2560
//  AS7265 spectral triad for colour and intencity detection
//  LM380 as opamp

#include "SparkFun_AS7265X.h" 
AS7265X sensor;

int pin1=8;     //  reagent 1 pump
int pin2=9;     //  reagent 2 pump
int pin3=10;    // water  pump

float I0=19.49;

void setup() {
  Serial.begin(115200);
  Serial.println("AS7265x Spectral Triad Reading");

  if(sensor.begin() == false)
  {
    Serial.println("Sensor does not appear to be connected. Please check the wiring. Freezing...");
    while(1);
  }
  sensor.disableIndicator();    //turning off blue indicator of spectro 
  //sensor.enableBulb(0);         //turning on white led of spectro
  //Serial.println("only 645nm red");

  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  pinMode(pin3,OUTPUT);

  // fan for cooling 
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
  
  // spectro led on
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT); 
  digitalWrite(6,HIGH);
  digitalWrite(7,LOW);

  //  pump pipe filling
  analogWrite(pin1,153);
  analogWrite(pin2,153);
  analogWrite(pin3,153);
  delay(4600);
  analogWrite(pin3,0);
  delay(900);
  analogWrite(pin1,0);
  analogWrite(pin2,0);
  delay(1000);
  
  //  chemical pouring into spectro beaker
  //  12ml reagent 1, 12ml reagent 2, 7ml sample water
  analogWrite(pin3,135);
  analogWrite(pin1,135);
  analogWrite(pin2,135);
  delay(30100);
  analogWrite(pin3,0);
  delay(24800);
  analogWrite(pin1,0);
  analogWrite(pin2,0);

}

void loop() {
  //  measurments in spectro beaker
  sensor.takeMeasurements();

  float I_min;
  while(1){
    float I1=sensor.getCalibratedJ();    //645nm red
    delay(10000);
    float I2=sensor.getCalibratedJ();    //645nm red
    if(I2>I1){
      I_min=I1;
      break;
    }else if(I2<I1){
      continue;
    }else if(I2==I1){
      I1=I2;
      delay(60000);
      float I2=sensor.getCalibratedJ();    //645nm red
      if(I1==I2){
        I_min=I1;
        break;
      }      
    }
  }
  
  double ratio=I0/I_min;
  double A=log10(ratio);
  double C=32.895*A;    //from calibration curve
  Serial.println(C);

}