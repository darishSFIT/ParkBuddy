# ParkBuddy
### Semester 7 IOT project using NodeMCU (ESP8266) board 

#### We used IR sensors to dectect slot occupancy

# Parking Spot Monitoring System

This project uses an ESP8266 microcontroller to monitor a parking spot's occupancy status and control a gate using a servo motor. The data is sent to Firebase Realtime Database, and the system includes indicators (LEDs) to show the status of the parking spot.

## Components Used

- ESP8266 Microcontroller
- IR Sensors
- LEDs
- Servo Motor
- Wi-Fi

## Libraries

- `ESP8266WiFi.h` - For connecting the ESP8266 to Wi-Fi.
- `ESP8266Firebase.h` - For interacting with the Firebase Realtime Database.
- `Servo.h` - For controlling the servo motor.

## Code Explanation

```cpp
#include <ESP8266WiFi.h>              // Include the ESP8266 Wi-Fi library for network communication.
#include <ESP8266Firebase.h>          // Include the Firebase library for interaction with Firebase.
#include <Servo.h>                    // Include the Servo library for controlling the servo motor.

// Firebase configuration
#define _SSID "Pixel 7A"              // Your Wi-Fi SSID
#define _PASSWORD "87654321"          // Your Wi-Fi Password
#define REFERENCE_URL "https://parkbuddy-653a1-default-rtdb.firebaseio.com"  // Firebase project URL

// Define pin numbers
#define GATE_IR_PIN D1                 // Gate IR sensor connected to pin D1
#define SLOT1_IR_PIN D2                // Parking spot 1 IR sensor connected to pin D2
#define SLOT1_RED_LED_PIN D5           // Red LED for slot 1 connected to pin D5
#define SLOT1_GREEN_LED_PIN D6         // Green LED for slot 1 connected to pin D6
#define SERVO_PIN D0                   // Servo motor connected to pin D0

// Initialize Firebase
Firebase firebase(REFERENCE_URL);      // Create a Firebase instance with the database URL.
Servo gateServo;                      // Create a Servo instance for controlling the gate.

void setup() {
  Serial.begin(115200);                // Initialize serial communication at 115200 baud.
  
  // Initialize LED and servo pins
  pinMode(LED_BUILTIN, OUTPUT);        // Set the built-in LED pin as an output.
  pinMode(SLOT1_RED_LED_PIN, OUTPUT);  // Set the red LED pin as an output.
  pinMode(SLOT1_GREEN_LED_PIN, OUTPUT);// Set the green LED pin as an output.

  // Setup Wi-Fi connection
  WiFi.mode(WIFI_STA);                 // Set Wi-Fi mode to station.
  WiFi.disconnect();                  // Disconnect from any previous Wi-Fi connections.
  delay(1000);                         // Wait for 1 second.
  Serial.print("Connecting to: ");
  Serial.println(_SSID);              // Print the SSID being connected to.
  WiFi.begin(_SSID, _PASSWORD);      // Begin the Wi-Fi connection.

  while (WiFi.status() != WL_CONNECTED) { // Wait until connected to Wi-Fi.
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected");   // Print a message when connected to Wi-Fi.
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());     // Print the local IP address of the ESP8266.

  // Initialize the servo
  gateServo.attach(SERVO_PIN);        // Attach the servo motor to the specified pin.
  gateServo.write(0);                 // Set the initial position of the servo (closed gate).

  // Firebase setup
  firebase.setString("Example/setString", "It's Working");   // Set a test string value in Firebase.
  firebase.setInt("Example/setInt", 123);                    // Set a test integer value in Firebase.
  firebase.setFloat("Example/setFloat", 45.32);              // Set a test float value in Firebase.
  
  firebase.pushString("push", "Hello");                       // Push a string value to Firebase.
  firebase.pushInt("push", 789);                              // Push an integer value to Firebase.
  firebase.pushFloat("push", 89.54);                          // Push a float value to Firebase.

  // Retrieve and print data from Firebase
  String data1 = firebase.getString("Example/setString");     // Get the string value from Firebase.
  Serial.print("Received String: ");
  Serial.println(data1);                                     // Print the received string value.

  int data2 = firebase.getInt("Example/setInt");              // Get the integer value from Firebase.
  Serial.print("Received Int: ");
  Serial.println(data2);                                     // Print the received integer value.

  float data3 = firebase.getFloat("Example/setFloat");        // Get the float value from Firebase.
  Serial.print("Received Float: ");
  Serial.println(data3);                                     // Print the received float value.

  firebase.deleteData("Example");                             // Delete the test data from Firebase.
}

void loop() {
  // Variables to track parking spot status
  bool isSlot1Occupied = (digitalRead(SLOT1_IR_PIN) == LOW); // Check if the parking spot is occupied by IR sensor.

  // Check gate IR sensor
  if (digitalRead(GATE_IR_PIN) == LOW) {  // If an object is detected by the gate IR sensor.
    gateServo.write(90);                 // Open the gate.
    delay(5000);                         // Keep the gate open for 5 seconds.
    gateServo.write(0);                  // Close the gate.
  }

  // Update LEDs based on parking spot status
  if (isSlot1Occupied) {
    digitalWrite(SLOT1_RED_LED_PIN, LOW);   // Turn off the red LED if the slot is occupied.
    digitalWrite(SLOT1_GREEN_LED_PIN, HIGH);// Turn on the green LED if the slot is occupied.
    firebase.setString("parking_spot/slot1/status", "occupied"); // Update Firebase with the slot status.
  } else {
    digitalWrite(SLOT1_RED_LED_PIN, HIGH);  // Turn on the red LED if the slot is empty.
    digitalWrite(SLOT1_GREEN_LED_PIN, LOW); // Turn off the green LED if the slot is empty.
    firebase.setString("parking_spot/slot1/status", "empty"); // Update Firebase with the slot status.
  }

  // Delay to avoid flooding the Firebase database with updates
  delay(5000); // Update every 5 seconds.
}
```
