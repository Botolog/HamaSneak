#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "s20";
const char* password = "12345678"; //08111973
const char* dropboxToken = "sl.BxDEltkdicfi3NZ9ZROqOxIHxlseBAaJzychpG9yBQE0h_7gXRwyzfK4FBir9xQPtI-MmE55JB1lf3N0XCen5rZ-pvsOPEMqbDZuPGVe_H-TC74gTBEQ0gDu_Xc67AYbTZjb3iw8bPuA";
const char* filePath = "/test.txt"; // Change the file path as per your requirement
const char* content = "Hello, Dropbox!"; // Content to be written to the file

// HTTPClient http;

void setup() {
  Serial.begin(115200);
  delay(4000); // Delay to ensure Serial monitor is ready
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  // Create a file in Dropbox
  // http.begin("https://google.com/");

  createDropboxFile("/yello.txt", "yo yo madafaka this is FUCKING WORKING!!!!");
  // Serial.println(http.GET());
}

void loop() {
  // Nothing to do in the loop for this example
}

void createDropboxFile(const char* path, const char* content) {
  HTTPClient http;
  
  // Construct the Dropbox API URL for file upload
  String url = "https://content.dropboxapi.com/2/files/upload";

  http.begin(url);
  http.addHeader("Authorization", "Bearer " + String(dropboxToken));
  http.addHeader("Content-Type", "application/octet-stream");
  http.addHeader("Dropbox-API-Arg", "{\"path\": \"" + String(path) + "\",\"mode\": \"add\",\"autorename\": true,\"mute\": false}");

  // Send POST request
  int httpResponseCode = http.POST(content);
  
  // Check for successful response
  if (httpResponseCode > 0) {
    Serial.print("Dropbox response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println("Dropbox response: " + response);
  } else {
    Serial.print("Error in Dropbox request, HTTP response code: ");
    Serial.println(httpResponseCode);
  }
  
  // Close connection
  http.end();
}
