#include <WiFi.h>
 
const char* ssid     = "Modin-Students";
const char* password = "";
 
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Connect to Wi-Fi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("New client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String request = client.readStringUntil(';');
  Serial.println(request);
  client.flush();
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  // client.println("YELLO");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1>Hello from Arduino!</h1>");
  client.println("</html>");
  
  delay(1);
  Serial.println("Client disconnected");
}
