#include <WiFi.h>

const char* ssid = "Kaluga 2.4";
const char* password = "41931047";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Connect to Wi-Fi
  Serial.println();
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected."); Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    while (client.connected()) {
      if (client.available()) {
        String command = client.readStringUntil(';');
        Serial.println("Received command: " + command);
        // Process the command and control the car accordingly
        // Example: controlMotors(command);
      }
    }
    Serial.println("Client disconnected");
  }
}
