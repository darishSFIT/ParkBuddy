#include <ESP8266WiFi.h>
#include <ESP8266Firebase.h>
#include <Servo.h>

// Firebase configuration
#define _SSID "Pixel 7a"            // Your WiFi SSID
#define _PASSWORD "87654321"        // Your WiFi Password
#define REFERENCE_URL "https://parkbuddy-653a1-default-rtdb.firebaseio.com"  // Your Firebase project reference URL

// Define pin numbers
#define GATE_IR_PIN D1              // Gate IR sensor connected to D1
#define SLOT1_IR_PIN D2             // Parking spot 1 IR sensor connected to D2
#define SLOT2_IR_PIN D3             // Parking spot 2 IR sensor connected to D3
#define SLOT3_IR_PIN D7             // Parking spot 3 IR sensor connected to D7

//#define SLOT1_RED_LED_PIN D5        // Slot 1 Red LED connected to D5
//#define SLOT1_GREEN_LED_PIN D6      // Slot 1 Green LED connected to D6
//#define SLOT2_RED_LED_PIN D8        // Slot 2 Red LED connected to D8
//#define SLOT2_GREEN_LED_PIN D9      // Slot 2 Green LED connected to D9
//#define SLOT3_RED_LED_PIN D10       // Slot 3 Red LED connected to D10
//#define SLOT3_GREEN_LED_PIN D11     // Slot 3 Green LED connected to D11

#define SERVO_PIN D0                // Servo motor connected to D0

// Initialize Firebase
Firebase firebase(REFERENCE_URL);
Servo gateServo;

void setup() {
  Serial.begin(115200);

  // Initialize LEDs and servo
  pinMode(LED_BUILTIN, OUTPUT);
//  pinMode(SLOT1_RED_LED_PIN, OUTPUT);
//  pinMode(SLOT1_GREEN_LED_PIN, OUTPUT);
//  pinMode(SLOT2_RED_LED_PIN, OUTPUT);
//  pinMode(SLOT2_GREEN_LED_PIN, OUTPUT);
//  pinMode(SLOT3_RED_LED_PIN, OUTPUT);
//  pinMode(SLOT3_GREEN_LED_PIN, OUTPUT);
  
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
}

void loop() {
  // Variables to track parking spot status
  bool isSlot1Occupied = (digitalRead(SLOT1_IR_PIN) == LOW); // Assuming LOW means object detected
  bool isSlot2Occupied = (digitalRead(SLOT2_IR_PIN) == LOW); // Assuming LOW means object detected
  bool isSlot3Occupied = (digitalRead(SLOT3_IR_PIN) == LOW); // Assuming LOW means object detected

  // Check gate IR sensor
  if (digitalRead(GATE_IR_PIN) == LOW) { // Assuming LOW means object detected
    Serial.println("Gate opened");
    firebase.setString("parking_space/gate/status", "Gate opened");


    //open gate slowly
    for (int angle = 80; angle >= 0; angle--) {
      gateServo.write(angle);
      delay(40);
    }
    //gateServo.write(90); // Open the gate

    
    
  } else {
    firebase.setString("parking_space/gate/status", "Gate closed");
    Serial.println("Gate closed");



    //close gate slowly
    delay(2000);
    for (int angle = 0; angle <= 80; angle++) {
      gateServo.write(angle);
      delay(40);
    }

    
//    delay(5000); // Keep the gate open for 5 seconds
//    gateServo.write(0); // Close the gate
  }

  // Update LEDs and Firebase for Slot 1
  if (isSlot1Occupied) {
//    digitalWrite(SLOT1_RED_LED_PIN, LOW);   // Turn off red LED
//    digitalWrite(SLOT1_GREEN_LED_PIN, HIGH); // Turn on green LED
    firebase.setString("parking_space/slot1/status", "Parked");
    Serial.println("Slot 1: occupied");
  } else {
//    digitalWrite(SLOT1_RED_LED_PIN, HIGH);  // Turn on red LED
//    digitalWrite(SLOT1_GREEN_LED_PIN, LOW); // Turn off green LED
    firebase.setString("parking_space/slot1/status", "Free");
    Serial.println("Slot 1: empty");
  }

  // Update LEDs and Firebase for Slot 2
  if (isSlot2Occupied) {
//    digitalWrite(SLOT2_RED_LED_PIN, LOW);   // Turn off red LED
//    digitalWrite(SLOT2_GREEN_LED_PIN, HIGH); // Turn on green LED
    firebase.setString("parking_space/slot2/status", "Parked");
    Serial.println("Slot 2: occupied");
  } else {
//    digitalWrite(SLOT2_RED_LED_PIN, HIGH);  // Turn on red LED
//    digitalWrite(SLOT2_GREEN_LED_PIN, LOW); // Turn off green LED
    firebase.setString("parking_space/slot2/status", "Free");
    Serial.println("Slot 2: empty");
  }

  // Update LEDs and Firebase for Slot 3
  if (isSlot3Occupied) {
//    digitalWrite(SLOT3_RED_LED_PIN, LOW);   // Turn off red LED
//    digitalWrite(SLOT3_GREEN_LED_PIN, HIGH); // Turn on green LED
    firebase.setString("parking_space/slot3/status", "Parked");
    Serial.println("Slot 3: occupied");
  } else {
//    digitalWrite(SLOT3_RED_LED_PIN, HIGH);  // Turn on red LED
//    digitalWrite(SLOT3_GREEN_LED_PIN, LOW); // Turn off green LED
    firebase.setString("parking_space/slot3/status", "Free");
    Serial.println("Slot 3: empty");
  }
  Serial.println("-----------");

  // Delay to avoid flooding the Firebase database with updates
  delay(5000); // Update every 5 seconds
}
