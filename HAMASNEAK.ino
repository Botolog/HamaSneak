bool EN_MOTORS = false, EN_DISPLAY = false, EN_RGB = false, EN_GYRO = false, EN_IR = false, EN_WIFI = false, EN_SCANNER = false, EN_SERVO = false;
//* Motors
#define Rplus 12
#define Rminus 13
#define Lplus 4
#define Lminus 15

const int freq = 3000;
const int resolution = 8;

void setupMotors()
{
  EN_MOTORS = true;
  ledcSetup(5, freq, resolution);
  ledcSetup(6, freq, resolution);
  ledcSetup(7, freq, resolution);
  ledcSetup(8, freq, resolution);
  ledcAttachPin(Rplus, 5);
  ledcAttachPin(Rminus, 6);
  ledcAttachPin(Lplus, 7);
  ledcAttachPin(Lminus, 8);
}

void SetSpeed(bool right, float speed = 0)
{
  speed = max(min((speed + 100) / 200 * 255, 255.0F), 0.0F);
  Serial.println(speed);

  if (right)
  {
    ledcWrite(5, speed);
    ledcWrite(6, 255 - speed);
  }
  else
  {
    ledcWrite(7, speed);
    ledcWrite(8, 255 - speed);
  }
}

void drive(int speed, int duration = 0, float shift = 0)
{
  SetSpeed(true, speed + shift);
  SetSpeed(false, speed - shift);
  delay(duration);
}

void stop()
{
  ledcWrite(5, 0);
  ledcWrite(6, 0);
  ledcWrite(7, 0);
  ledcWrite(8, 0);
}

void turn(int speed, float targetAngel = 90)
{
  resetAngel();
  unsigned long now = millis();
  while (targetAngel > getAngel())
  {
    float shiftAngle = targetAngel - (getAngel());
    float goin = speed * shiftAngle * 0.02;
    SetSpeed(true, -goin - 10 * (abs(shiftAngle) / shiftAngle));
    SetSpeed(false, goin + 10 * (abs(shiftAngle) / shiftAngle));
    delay(10);
  }
  stop();
}

//* MPU6050
#include <Wire.h> // type: ignore
#include <MPU6050.h>

MPU6050 mpu;
unsigned long lastTime = 0;
float gyroAngleX = 0;
float resetedAngel = 0;

void setupGyro()
{
  EN_GYRO = true;
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

float getAngel()
{
  if (EN_GYRO)
  {
    return getRawAngel() - resetedAngel;
  }
  else
  {
    Serial.println("pls turn on gyro!!");
    ESP.restart();
  }
}

void resetAngel()
{
  if (EN_GYRO)
  {
    resetedAngel = getRawAngel();
  }
  else
  {
    Serial.println("pls turn on gyro!!");
    ESP.restart();
  }
}

void driveP(int speed, int duration = 0, float targetAngel = 0)
{
  duration = duration * 1000;
  resetAngel();
  unsigned long now = millis();
  while (now + duration > millis())
  {
    float shiftAngle = getAngel();
    SetSpeed(true, speed + shiftAngle + 10 * (abs(shiftAngle) / shiftAngle));
    SetSpeed(false, speed - shiftAngle - 10 * (abs(shiftAngle) / shiftAngle));
    delay(10);
  }
}

//* Screen
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setupDisplay()
{
  EN_DISPLAY = true;
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("I Love U Ofir <3");
  display.display();
  delay(5000);
}

void printOnDisplay(String text, bool clear = true)
{
  if (clear)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
  }
  display.println(text);
  display.display();
}

//* RGB led
#define Rled 26
#define Gled 27
#define Bled 2

void setupRGB()
{
  EN_RGB = true;
  pinMode(Rled, OUTPUT);
  pinMode(Gled, OUTPUT);
  pinMode(Bled, OUTPUT);
}

void RGB(bool R, bool G, bool B)
{
  digitalWrite(Rled, R);
  digitalWrite(Gled, G);
  digitalWrite(Gled, B);
}

void generateR(int i) {}

//* Servos
#include <Servo.h>

Servo Xservo;
Servo Yservo;

void setupServo()
{
  EN_SERVO = true;
  pinMode(25, OUTPUT);
  Xservo.attach(23);
  Yservo.attach(2);
  Yservo.write(0);
  Xservo.write(0);
}

int XservoPos = 0, YservoPos = 0;

void targetServo(int Xtarget, int Ytarget)
{
  while (abs(Xtarget - XservoPos) + abs(Ytarget - YservoPos) > 2)
  {
    int dx = abs(Xtarget - XservoPos) / (Xtarget - XservoPos);
    int dy = abs(Ytarget - YservoPos) / (Ytarget - YservoPos);
    Xservo.write(XservoPos + dx);
    XservoPos = XservoPos + dx;
    Yservo.write(YservoPos + dy);
    YservoPos = YservoPos + dy;
    delay(20);
  }
}

//* IR
#define RIR 34
#define LIR 35

void setupIR()
{
  EN_IR = true;
  pinMode(RIR, INPUT);
  pinMode(LIR, INPUT);
}

float readIR(bool right, bool analog = true)
{
  if (!EN_IR)
  {
    Serial.println("pls turn on IR!!");
    ESP.restart();
  }
  if (analog)
  {
    if (right)
    {
      return 100 - analogRead(RIR) / -40.95;
    }
    if (!right)
    {
      return 100 - analogRead(LIR) / -40.95;
    }
  }
  else
  {
    if (right)
    {
      return analogRead(RIR) < 500;
    }
    if (!right)
    {
      return analogRead(LIR) < 500;
    }
  }
}

void driveIR(int speed, int duration = 0, float hardness = 0.3, float Ddrift = 0.08)
{
  duration = duration * 1000;
  unsigned long now = millis();
  float drift = 0;
  while (now + duration > millis())
  {
    float IRRval = readIR(true);
    float IRLval = readIR(false);
    drive(speed, 0, (IRLval - IRRval) * (hardness + drift));
    delay(20);
    drift += Ddrift;
    if (abs(IRLval - IRRval) < 10)
    {
      drift = 0;
    }
  } // driveIR(100, 5000, 0.8, 0.05);  for hard angles like rectanle
    // driveIR(100, 5000, 0.6, 0.02); for soft angles like a little curve
}

//* WiFi & HTTP
#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "Kaluga 2.4";
const char *password = "41931047";

WiFiServer server(80);

String BaseURL = "";

void setupWiFi()
{
  EN_WIFI = true;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("");
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
  if (EN_DISPLAY)
  {
    printOnDisplay(String(WiFi.localIP()));
  }
}

void uploadRout(char act, int Size = 0)
{
  HTTPClient http;
  String str = BaseURL + String(act) + "/" + String(Size);
  Serial.println(str);
  http.begin(str);
  int httpCode = http.GET();
  if (httpCode > 0)
  {

    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
  }

  else
  {
    Serial.print("Error on HTTP request: ");
    Serial.println(httpCode);
  }

  http.end(); // Free the resources
  // uploadRout('S'); SCAN
  // uploadRout('F', 10); FORWARD 10cm
}

int contX;
int contY;
int camX;
int camY;
int option;

void sep(String a)
{
  a.remove(0, 1);
  a.remove(a.length() - 1);

  // Split the string by ","
  int commaIndex1 = a.indexOf(',');
  String b = a.substring(0, commaIndex1); // Extract substring before the first comma

  // Find the second comma after the first one
  int commaIndex2 = a.indexOf(',', commaIndex1 + 1);
  String c = a.substring(commaIndex1 + 1, commaIndex2); // Extract substring between first and second comma

  // Find the third comma after the second one
  int commaIndex3 = a.indexOf(',', commaIndex2 + 1);
  String d = a.substring(commaIndex2 + 1, commaIndex3); // Extract substring between second and third comma

  // Find the fourth comma after the third one
  int commaIndex4 = a.indexOf(',', commaIndex3 + 1);
  String e = a.substring(commaIndex3 + 1, commaIndex4); // Extract substring between third and fourth comma
  String f = a.substring(commaIndex4 + 1);              // Extract substring after the fourth comma

  contX = b.toInt();
  contY = c.toInt();
  camX = d.toInt();
  camY = e.toInt();
  option = f.toInt();
}

void remoteCtrl()
{
  server.begin();
  String command = "N";
  WiFiClient client = server.available();
  Serial.println("rmtctrl started");
  if (client)
  {
    Serial.println("New client connected");
    while (client.connected() || command == "/")
    {
      if (client.available())
      {
        command = client.readStringUntil(';');
        if (command.indexOf("/") != -1)
        {
          Serial.println("Got IP: " + command);
          BaseURL = command;
        }
        Serial.println("Received command: " + command);
        sep(command);
        int speed = contY, shift = contX;
        if (speed != 0 || shift != 0)
        {
          drive(speed, 0, shift);
        }
        else
        {
          stop();
        }
        if (EN_SERVO)
        {
          targetServo(camX, camY);
        }
        if (option == 1)
        {
          digitalWrite(25, HIGH);
        }
        else if (option == 0)
        {
          digitalWrite(25, LOW);
        }
        else if (option > 1)
        {
          scan();
        }
      }
    }
  }
  Serial.println("Client disconnected");
}

//* Scanner (Servo+TFL)
#include <TFLI2C.h>

TFLI2C tflI2C;
int16_t tfDist;               // distance in centimeters
int16_t tfAddr = TFL_DEF_ADR; // Use this default I2C address

int xdots[180];
int ydots[180];

float K = 0.199 + 0.801;

void setupScanner()
{
  EN_SCANNER = true;
  while (0)
  {
    delay(1500);
    for (int test = 0; test <= 180; test += 1)
    {
      delay(10);
      Xservo.write(test * K);
      Serial.println(test);
    }
    delay(500);
    Xservo.write(0);
    delay(500);
  }
}

int16_t mesure()
{
  if (!EN_SCANNER)
  {
    Serial.println("pls turn on scanner!!");
    ESP.restart();
  }
  delay(10);
  tflI2C.getData(tfDist, tfAddr);
  Serial.println(tfDist);
  return tfDist;
}

int XServo_position = 0;
int dx = 1;
int dy = 1;
double t = 1.2;

void scan()
{
  String toSend[180];
  Xservo.write(XServo_position * K);
  for (int YServo_position = 0; YServo_position < 180; YServo_position += dy)
  {
    if (true)
    { //(XServo_position < 90){
      for (XServo_position = 0; XServo_position < 180; XServo_position += dx)
      {
        Xservo.write(XServo_position * K);
        xdots[XServo_position] = XServo_position;
      }
    }
    else
    {
      for (XServo_position = 180; XServo_position > 0; XServo_position -= dx)
      {
        Xservo.write(XServo_position * K);
        xdots[XServo_position] = mesure();
      }
    }
    Yservo.write(YServo_position);
    Xservo.write(0);
    uploadScan(YServo_position, xdots);
    String toAdd = "";

    delay(20);
  }
  
}

void uploadScan(int pos, int theScan[180])
{
  HTTPClient http;
  String Line = "";
  for (int i=0; i<179; i++){
    Line += theScan[i] + ",";
  }
  Line += theScan[179];

    String url = BaseURL + "US/" + pos + "/" + Line;
    http.begin(url);
    int httpResponseCode = http.GET();
    http.end();
}

//* Main
void setup()
{
  Serial.begin(115200);
  setupMotors();
  setupServo();
  // setupDisplay();
  // setupRGB();
  // setupGyro();
  setupIR();
  setupWiFi();
  setupScanner();

  Serial.println("Ready! Starting...");
  delay(500);
}

void loop()
{
  Serial.println("starting rmtctrl...");
  remoteCtrl();
  Serial.println("rmtctrl ended");
  delay(5000);
  // driveIR(90, 5000, 0.3, 0.08);
}
