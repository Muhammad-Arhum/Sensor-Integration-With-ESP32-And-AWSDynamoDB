#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// AWS endpoint details
const String aws_endpoint = "DynamoDB_EndPoint_Link";
const String access_key = "Your_Access_Key";
const String secret_key = "Your_Secret_Access_Key";

// Pin Definitions for SR-04
#define TRIG_PIN 5
#define ECHO_PIN 18

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected!");
}

void loop() {
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // Send data to AWS DynamoDB
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(aws_endpoint);
    http.addHeader("Content-Type", "application/json");
    
    String postData = "{\"distance\":\"" + String(distance) + "\"}";
    
    int httpResponseCode = http.POST(postData);
    
    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully!");
    } else {
      Serial.println("Error sending data: " + String(httpResponseCode));
    }
    
    http.end();
  }
  
  delay(5000);  // Wait for 5 seconds before the next measurement
}
