#include <WiFi.h>
#include <HTTPClient.h>

String ACCESS_TOKEN = "sl.BxDEltkdicfi3NZ9ZROqOxIHxlseBAaJzychpG9yBQE0h_7gXRwyzfK4FBir9xQPtI-MmE55JB1lf3N0XCen5rZ-pvsOPEMqbDZuPGVe_H-TC74gTBEQ0gDu_Xc67AYbTZjb3iw8bPuA";
const char *ssid = "Kaluga 2.4";
const char *password = "41931047";

HTTPClient http;
HTTPClient httpClient = HttpClient(wifiClient, host, port);
String BaseURL = "http://10.0.0.55:8080/";

void setupWiFi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
}

void uploadScan(int arrnum, int arr[180])
{
  String str = BaseURL + String(arrnum) + "/";
  for (int i = 0; i < 179; i++)
  {
    str += String(arr[i]) + ",";
  }
  str += String(arr[179]);
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
}

void uploadRout(char act, int Size = 0)
{
  String str = BaseURL + "UR/" + String(act) + "/" + String(Size);
  Serial.println(str);
  http.begin(str);
  int httpCode = http.GET();
  if (httpCode > 0)
  { // Check for the returning code

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

void createFile()
{
  HTTPClient http;

  // Specify request destination
  String url = "https://content.dropboxapi.com/2/files/upload";

  // Set headers
  http.addHeader("Authorization", "Bearer " + ACCESS_TOKEN);
  http.addHeader("Content-Type", "application/octet-stream");
  http.addHeader("Dropbox-API-Arg", "{\"path\":\"yello.tes\", \"mode\":\"add\", \"autorename\":true, \"mute\":false}");

  // JSON body
  String body = "{\"key1\": \"value1\", \"key2\": \"value2\"}";

  // Send POST request
  Serial.print("Sending POST request to: ");
  Serial.println(url);
  int httpCode = http.POST(body);
  Serial.println(httpCode);
}

void createFileInDropbox() {
  Serial.println("Creating file in Dropbox...");

  String content = "Hello, Dropbox!"; // Content of the file to be created

  String requestBody = "{\"path\": \"/example.txt\",\"mode\": \"add\",\"autorename\": true,\"mute\": false,\"strict_conflict\": false,\"contents\": \"" + content + "\"}";

  httpClient.beginRequest();
  httpClient.post("/2/files/upload", "application/json", requestBody);
  httpClient.sendHeader("Authorization", "Bearer " + String(dropboxToken));
  httpClient.sendHeader("Dropbox-API-Arg", "{\"path\": \"/example.txt\",\"mode\": \"add\",\"autorename\": true,\"mute\": false,\"strict_conflict\": false}");
  httpClient.sendHeader("Content-Length", String(requestBody.length()));
  httpClient.beginBody();
  httpClient.print(requestBody);
  httpClient.endRequest();

  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  Serial.println("Response:");
  Serial.println(statusCode);
  Serial.println(response);
}

void setup()
{
  Serial.begin(115200);
  setupWiFi();
  Serial.println("requesting...");
  delay(500);
  createFile();
  delay(10000);
}
void loop()
{
}