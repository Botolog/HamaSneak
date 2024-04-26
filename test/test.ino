#define LeftMotors 12 //13 
#define RightMotors 4 //15
#define LeftColor 36
#define RightColor 39
#define res 8
#define frq 5000 
void setup() {
  // put your setup code here, to run once:
  pinMode(LeftColor,INPUT);
  pinMode(RightColor,INPUT);
  ledcSetup(5, frq, res);
  //ledcSetup(6, freq, resolution);
  ledcSetup(7, frq, res);
  //ledcSetup(8, freq, resolution);
  ledcAttachPin(LeftColor, 5);
  //ledcAttachPin(Rminus, 6);
  ledcAttachPin(RightColor, 7);
  //ledcAttachPin(Lminus, 8);
}
void loop() {
  // put your main code here, to run repeatedly:
  int LC = analogRead(LeftColor);
  int RC = analogRead(RightColor);

  if(LeftColor<800){
    ledcWrite(RightMotors,50);
    ledcWrite(LeftMotors,0);
  }
  else if(RightColor<800){
    ledcWrite(RightMotors,0);
    ledcWrite(LeftMotors,50);
  }
  else{
    ledcWrite(RightMotors,0);
    ledcWrite(LeftMotors,0);
  }

}
