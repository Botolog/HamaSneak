// Motors
#define Rplus 12
#define Rminus 13
#define Lplus 4
#define Lminus 15

const int freq = 5000;
const int resolution = 8;

void setupMotors()
{
  ledcSetup(1, freq, resolution); ledcSetup(2, freq, resolution);
  ledcSetup(3, freq, resolution); ledcSetup(4, freq, resolution);
  ledcAttachPin(Rplus, 1); ledcAttachPin(Rminus, 2);
  ledcAttachPin(Lplus, 3); ledcAttachPin(Lminus, 4);
}

void SetSpeed(bool right, float speed = 0)
{
  speed = max(min((speed+100)/200*255, 255.0F), 0.0F);
  Serial.println(speed);

  if (right) {ledcWrite(1, speed); ledcWrite(2, 255-speed);}
  else {ledcWrite(3, speed); ledcWrite(4, 255-speed);}
}

void drive(int speed, int duration=0, float shift = 0)
{
  SetSpeed(true,  speed+shift);
  SetSpeed(false, speed-shift);
  delay(duration);
}

void stop()
{
  ledcWrite(1, 0); ledcWrite(2, 0);
  ledcWrite(3, 0); ledcWrite(4, 0);
}

void turn(int speed, float targetAngel = 90)
{
  resetAngel();
  unsigned long now = millis();
  while (targetAngel > getAngel())
  {
    float shiftAngle = targetAngel-(getAngel());
    float goin = speed*shiftAngle*0.02;
    SetSpeed(true, -goin - 10*(abs(shiftAngle)/shiftAngle));
    SetSpeed(false, goin + 10*(abs(shiftAngle)/shiftAngle));
    delay(10);
  }
  stop();
}

// MPU6050 
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
unsigned long lastTime = 0;
float gyroAngleX = 0;
float resetedAngel = 0; 

void setupGyro()
{
  Wire.begin();
  mpu.initialize();
  mpu.CalibrateGyro();
}

float getRawAngel()
{
  unsigned long now = millis();
  float elapsedTime = (now - lastTime) / 1000.0;
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  float gyroRateX = gx / 131.0;
  gyroAngleX += gyroRateX * elapsedTime;
  lastTime = now;
  return gyroAngleX;
}

float getAngel() {return getRawAngel()-resetedAngel;}

void resetAngel() {resetedAngel = getRawAngel();}

void driveP(int speed, int duration=0, float targetAngel = 0)
{
  duration = duration*1000;
  resetAngel();
  unsigned long now = millis();
  while (now + duration > millis())
  {
    float shiftAngle = getAngel();
    SetSpeed(true,  speed+shiftAngle + 10*(abs(shiftAngle)/shiftAngle));
    SetSpeed(false, speed-shiftAngle - 10*(abs(shiftAngle)/shiftAngle));
    delay(10);
  }
}

// Screen
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setupDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed")); for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("I Love U Ofir <3");
  display.display();  delay(5000);
}

void printOnDisplay(String text, bool clear=true) 
{
  if (clear) {
    display.clearDisplay(); display.setTextSize(2);
    display.setTextColor(WHITE);display.setCursor(0, 10);
  }
  display.println(text);
  display.display();
}

// RGB led
#define Rled 26
#define Gled 27
#define Bled 2

void setupRGB() {pinMode(Rled, OUTPUT); pinMode(Gled, OUTPUT); pinMode(Bled, OUTPUT);}

void RGB(bool R, bool G, bool B) {digitalWrite(Rled, R); digitalWrite(Gled, G); digitalWrite(Gled, B);}

void generateR(int i) {}

// IR
#define RIR 34
#define LIR 35

void setupIR(){
  pinMode(RIR, INPUT);
  pinMode(LIR, INPUT);
}

float readIR(bool right, bool analog=true){
  if (analog) {
    if (right)  {return 100 - analogRead(RIR) / -40.95;}
    if (!right) {return 100 - analogRead(LIR) / -40.95;}    
  }
  else {
    if (right)  {return analogRead(RIR) < 500;}
    if (!right) {return analogRead(LIR) < 500;}
  }
}

void driveIR(int speed, int duration = 0, float hardness = 0.5, float Ddrift = 0.05)
{
  duration = duration * 1000;
  unsigned long now = millis();
  float drift = 0;
  while (now + duration > millis())
  {
    float IRRval = readIR(true);
    float IRLval = readIR(false);
    drive(speed, 0, (IRLval - IRRval) * (hardness+drift));
    delay(20);
    drift += Ddrift;
    if (abs(IRLval - IRRval) < 10) {drift=0;}
  } // driveIR(100, 5000, 0.8, 0.05);  for hard angles like rectanle
    // driveIR(100, 5000, 0.6, 0.02); for soft angles like a little curve
}

// WiFi
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Kaluga 2.4";
const char* password = "41931047";

HTTPClient http;
String BaseURL = "https://GALER-TES.botologe.repl.co/US/";

void setupWiFi() {
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
}

void uploadScan(int arrnum, int arr[180]){
  String str = BaseURL+String(arrnum)+"/";
  for (int i=0; i<179; i++){
    str += String(arr[i]) +",";
  } 
  str += String(arr[179]);
  Serial.println(str);
  http.begin(str);
  int httpCode = http.GET();                                                 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpCode);
    }
 
    http.end(); //Free the resources
}

void uploadRout(char act, int Size=0){
  String str = BaseURL+String(act)+"/"+String(Size);
  Serial.println(str);
  http.begin(str);
  int httpCode = http.GET();                                                 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpCode);
    }
 
    http.end(); //Free the resources
    // uploadRout('S'); SCAN
    // uploadRout('F', 10); FORWARD 10cm
}

// Scanner (Servo+TFL)
#include <TFLI2C.h>
#include <Servo.h>

TFLI2C tflI2C;

Servo Xservo;  
Servo Yservo;  

int16_t  tfDist;    // distance in centimeters
int16_t  tfAddr = TFL_DEF_ADR;  // Use this default I2C address

int xdots[180];
int ydots[180]; 

float K = 0.801;

void setupSCANNER() {
  Xservo.attach(32); 
  Yservo.attach(33);  
  Yservo.write(0);
  Xservo.write(0);
  
  while (0){
  delay(1500);
   for (int test=0; test<=180; test+=1){
     delay(10);
     Xservo.write(test*K);
     Serial.println(test);
   }
   delay(500);
   Xservo.write(0);
   delay(500);
   }
}

int16_t mesure(){
  delay(5);
  tflI2C.getData(tfDist, tfAddr);
  return tfDist;
}

int XServo_position = 0;
int dx = 1;
int dy = 1;
double t = 1.2;

void scanAndUpload() {
  Xservo.write(XServo_position*K);
  for (int YServo_position = 0; YServo_position < 180; YServo_position+=dy){
    if (true){ //(XServo_position < 90){
      for (XServo_position = 0; XServo_position < 180; XServo_position+=dx) { 
        Xservo.write(XServo_position*K);
        xdots[XServo_position] = mesure();
      }
    }
    else {
      for (XServo_position = 180; XServo_position > 0; XServo_position-=dx) { 
        Xservo.write(XServo_position*K);
        xdots[XServo_position] = mesure();
      }
    }
    Yservo.write(YServo_position);
    Xservo.write(0); 
    uploadScan(YServo_position, xdots);
    for (int i=0; i<180; i++){Serial.print(xdots[i]); Serial.print(";");}
    Serial.print("-"); Serial.println(YServo_position);
    
    delay(500);
  }
}

// Main
void setup()
{
  Serial.begin(115200);
  setupMotors();
  // setupDisplay();
  // setupRGB();
  // setupGyro();
  setupIR();
  
  Serial.println("Ready! Starting...");
  delay(500);
}

void loop()
{
  driveIR(90, 5000, 0.3, 0.08);
}
