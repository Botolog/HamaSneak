#include <Arduino.h>
#define RightMotors 13
#define LeftMotors 4
#define RightColor 36
#define LeftColor 39
// setting PWM properties
const int PWMfreq = 5000;
const int RightMotorsChannel = 0;
const int LeftMotorsChannel = 1;
const int PWMRes = 8;

void setup() {
pinMode(RightColor,INPUT);
pinMode(LeftColor,INPUT);
ledcSetup(RightMotorsChannel, PWMfreq, PWMRes);
ledcSetup(LeftMotorsChannel, PWMfreq, PWMRes);
ledcAttachPin(RightMotors, RightMotorsChannel);
ledcAttachPin(LeftMotors, LeftMotorsChannel);
}
void loop() {
  int RColorValue = analogRead(RightColor);
  int LColorValue = analogRead(LeftColor);
  if(RColorValue<200){
    ledcWrite(LeftMotorsChannel, 25);
    ledcWrite(RightMotorsChannel, 0);
  }
  else if(LColorValue<200){
    ledcWrite(RightMotorsChannel, 25);
    ledcWrite(LeftMotorsChannel, 0);

  }
  else{
    ledcWrite(LeftMotorsChannel, 25);
    ledcWrite(RightMotorsChannel, 25);
  }

}

