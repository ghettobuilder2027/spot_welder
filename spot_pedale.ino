// Spot welder pedal v 0.1

#include <Button2.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "SPOT";
const char* password = "";
String command = "http://192.168.4.22/spot";
Button2 button;
WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(115200); 
  button.begin(2);
  button.setPressedHandler(pressed); 
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  button.loop(); 
}

void pressed(Button2& btn) {
  Serial.println("pressed");
  http.begin(client, command.c_str());
  int httpResponseCode = http.GET();
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}
