#include <ESP8266WiFi.h>
#include <ESP8266Firebase.h>
#include <Servo.h>

// Firebase configuration
#define _SSID "Pixel 7a"            // Your WiFi SSID
#define _PASSWORD "87654321"        // Your WiFi Password
#define REFERENCE_URL "https://parkbuddy-653a1-default-rtdb.firebaseio.com"  // Your Firebase project reference URL

// Define pin numbers
#define GATE_IR_PIN D1             // Gate IR sensor connected to D1
#define SLOT1_IR_PIN D2            // Parking spot 1 IR sensor connected to D2
#define SLOT1_RED_LED_PIN D5       // Slot 1 Red LED connected to D5
#define SLOT1_GREEN_LED_PIN D6     // Slot 1 Green LED connected to D6
#define SERVO_PIN D0               // Servo motor connected to D0

// Initialize Firebase
Firebase firebase(REFERENCE_URL);
Servo gateServo;

void setup() {
  Serial.begin(115200);

  // Initialize LED and servo
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SLOT1_RED_LED_PIN, OUTPUT);
  pinMode(SLOT1_GREEN_LED_PIN, OUTPUT);
  
  // Setup Wi-Fi connection
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize the servo
  gateServo.attach(SERVO_PIN);
  gateServo.write(0); // Initial position of the servo (closed gate)

  // Firebase setup
  firebase.setString("Example/setString", "It's Working");
  firebase.setInt("Example/setInt", 123);
  firebase.setFloat("Example/setFloat", 45.32);

  firebase.pushString("push", "Hello");
  firebase.pushInt("push", 789);
  firebase.pushFloat("push", 89.54);

  String data1 = firebase.getString("Example/setString");
  Serial.print("Received String: ");
  Serial.println(data1);

  int data2 = firebase.getInt("Example/setInt");
  Serial.print("Received Int: ");
  Serial.println(data2);

  float data3 = firebase.getFloat("Example/setFloat");
  Serial.print("Received Float: ");
  Serial.println(data3);

  firebase.deleteData("Example");
}

void loop() {
  // Variables to track parking spot status
  bool isSlot1Occupied = (digitalRead(SLOT1_IR_PIN) == LOW); // Assuming LOW means object detected

  // Check gate IR sensor
  if (digitalRead(GATE_IR_PIN) == LOW) { // Assuming LOW means object detected
    gateServo.write(90); // Open the gate
    delay(5000); // Keep the gate open for 5 seconds
    gateServo.write(0); // Close the gate
  }

  // Update LEDs based on parking spot status
  if (isSlot1Occupied) {
    digitalWrite(SLOT1_RED_LED_PIN, LOW); // Turn off red LED
    digitalWrite(SLOT1_GREEN_LED_PIN, HIGH); // Turn on green LED
    firebase.setString("parking_spot/slot1/status", "occupied");
  } else {
    digitalWrite(SLOT1_RED_LED_PIN, HIGH); // Turn on red LED
    digitalWrite(SLOT1_GREEN_LED_PIN, LOW); // Turn off green LED
    firebase.setString("parking_spot/slot1/status", "empty");
  }

  // Delay to avoid flooding the Firebase database with updates
  delay(5000); // Update every 5 seconds
}
