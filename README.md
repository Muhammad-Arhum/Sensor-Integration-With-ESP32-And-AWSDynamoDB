# Sensor-Integration-With-ESP32-And-AWSDynamoDB

1. Wi-Fi Setup

At the beginning of the code, the necessary Wi-Fi credentials are defined:

cpp

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

The ESP32 will connect to this network, which is essential for sending data to the AWS cloud. In the setup() function, the WiFi.begin() command attempts to establish the connection, and the loop waits until a successful connection is made, notifying you via the Serial Monitor.
2. SR-04 Ultrasonic Sensor Setup

The Trig and Echo pins of the SR-04 sensor are connected to the ESP32 and defined as:

cpp

#define TRIG_PIN 5
#define ECHO_PIN 18

These are configured as output and input pins, respectively. The digitalWrite and pulseIn functions are used to trigger the sensor and measure the time it takes for the ultrasonic pulse to return, which is then used to calculate the distance.
3. Distance Calculation

Within the loop(), the code calculates the distance based on the time it takes for the pulse to return:

cpp

long duration, distance;
duration = pulseIn(ECHO_PIN, HIGH);
distance = duration * 0.034 / 2;

This calculation is derived from the speed of sound in air (343 m/s), where the distance is halved because the ultrasonic wave travels to the object and back.
4. Sending Data to AWS DynamoDB

Once the distance is measured, the code uses the HTTPClient library to send the data to AWS DynamoDB. The connection to the AWS endpoint is established using the credentials:

cpp

HTTPClient http;
http.begin(aws_endpoint);
http.addHeader("Content-Type", "application/json");

The data is formatted in JSON, with the measured distance being sent in a POST request:

cpp

String postData = "{\"distance\":\"" + String(distance) + "\"}";
int httpResponseCode = http.POST(postData);

The http.POST function sends this JSON-formatted data to the specified endpoint. If successful, a confirmation message is printed to the Serial Monitor.
5. Data Transmission Timing

The loop sends data every 5 seconds, allowing for periodic updates to DynamoDB:

cpp

delay(5000);

This ensures regular distance measurements are sent, with a manageable delay between readings.
