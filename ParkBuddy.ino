#include <ESP8266WiFi.h>
#include <Servo.h>

// Define pin numbers
#define gateSensor D1
#define spotSensor1 D2
#define spotSensor2 D3
#define spotSensor3 D4

#define redLED1 D5
#define greenLED1 D6
#define redLED2 D7
#define greenLED2 D8

#define servoPin D9 // Make sure this pin is available and not used by default functions

// Create a Servo object
Servo gateServo;

// Wi-Fi credentials
const char* ssid = "Pixel 7a";
const char* password = "87654321";

// Cloud server URL
const char* serverURL = "http://your-cloud-database.com/update";

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize sensors and LEDs
  pinMode(gateSensor, INPUT);
  pinMode(spotSensor1, INPUT);
  pinMode(spotSensor2, INPUT);
  pinMode(spotSensor3, INPUT);

  pinMode(redLED1, OUTPUT);
  pinMode(greenLED1, OUTPUT);
  pinMode(redLED2, OUTPUT);
  pinMode(greenLED2, OUTPUT);

  // Attach the servo motor
  gateServo.attach(servoPin);
  gateServo.write(0); // Initial position (closed gate)

  // Connect to Wi-Fi
  connectToWiFi();
}

void loop() {
  // Check for object at the gate
  if (digitalRead(gateSensor) == HIGH) {
    openGate();
  }

  // Check the status of each parking spot
  checkParkingSpot(spotSensor1, redLED1, greenLED1, 1);
  checkParkingSpot(spotSensor2, redLED2, greenLED2, 2);
  checkParkingSpot(spotSensor3, redLED3, greenLED3, 3);

  // Add a small delay to prevent overwhelming the system
  delay(500);
}

void connectToWiFi() {
  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");
}

void openGate() {
  // Rotate the servo to open the gate
  gateServo.write(90);  // Open position
  delay(5000); // Keep the gate open for 5 seconds
  gateServo.write(0); // Close the gate
}

void checkParkingSpot(int sensorPin, int redLED, int greenLED, int spotID) {
  if (digitalRead(sensorPin) == HIGH) {
    // Spot is occupied
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    sendDataToCloud(spotID, "occupied");
  } else {
    // Spot is empty
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    sendDataToCloud(spotID, "empty");
  }
}

void sendDataToCloud(int spotID, String status) {
  if (WiFi.status() == WL_CONNECTED) {
    // Send data to the cloud
    WiFiClient client;
    if (client.connect(serverURL, 80)) {
      String postData = "spotID=" + String(spotID) + "&status=" + status;
      client.println("POST /update HTTP/1.1");
      client.println("Host: your-cloud-database.com");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println("Content-Length: " + String(postData.length()));
      client.println();
      client.println(postData);
    }
    client.stop();
  }
}
