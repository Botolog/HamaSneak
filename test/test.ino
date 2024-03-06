#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Kaluga 2.4";
const char* password = "41931047";

HTTPClient http;
String BaseURL = "https://hamasneak.pages.dev/";

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

void setup()
{
  Serial.begin(115200);
  setupWiFi();
  Serial.println("requesting..."); delay(500);
  uploadRout('R', 10);
  delay(10000);
}
void loop(){
  
}