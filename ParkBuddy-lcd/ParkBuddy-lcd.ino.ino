#include <ESP8266WiFi.h>
#include <ESP8266Firebase.h>
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_PCF8574.h>
//#include <LiquidCrystal_I2C.h>


// Wi-Fi configuration
#define _SSID "Pixel 7a"       // Your WiFi SSID
#define _PASSWORD "87654321"   // Your WiFi Password

// Firebase configuration
#define REFERENCE_URL "https://parkbuddy-653a1-default-rtdb.firebaseio.com"

// Define pin numbers
#define GATE_IR_PIN D1    // Gate IR sensor connected to D1
#define SERVO_PIN D0      // Servo motor connected to D0
#define SLOT1_IR_PIN D2   // Parking spot 1 IR sensor connected to D2
#define SLOT2_IR_PIN D3   // Parking spot 2 IR sensor connected to D3
#define SLOT3_IR_PIN D7   // Parking spot 3 IR sensor connected to D7

// Initialize Firebase
Firebase firebase(REFERENCE_URL);
Servo gateServo;

// Define LCD I2C address and pin setup for ESP8266
LiquidCrystal_PCF8574 lcd(0x27);  // Adjust the address as needed (0x27 or 0x3F)

void setup() {
  Serial.begin(115200);

  // Initialize Wi-Fi connection
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }

  Serial.println("WiFi Connected: ");
  Serial.println(_SSID);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize the servo
  gateServo.attach(SERVO_PIN);
  gateServo.write(0); // Initial position of the servo (closed gate)

  // Initialize the LCD
  Wire.begin(D6, D5);    // SDA -> D2 (GPIO4), SCL -> D1 (GPIO5)
  lcd.begin(16, 2);      // Initialize LCD with 16 columns and 2 rows
  lcd.setBacklight(255); // Turn on the backlight at full brightness

  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
}

void loop() {

  if (digitalRead(GATE_IR_PIN) == HIGH) { 
    checkGate();  // Check and update the gate status
    checkSlots(); // Check and update all parking slots simultaneously    
  } 
  else {
    Serial.println("Entry detected");
    gateServo.attach(SERVO_PIN);
    checkGate();
    checkSlots();       

    //open gate slowly
    for (int angle = 0; angle <= 100; angle+=4) {
      gateServo.write(angle);
      delay(20);
    }

    //keep gate open until the vehicle is waiting at gate
    while (digitalRead(GATE_IR_PIN) != HIGH) {
      checkGate();
      checkSlots();
    }

    //close gate slowly
    checkGate();
    checkSlots();   
       
    for (int angle = 100; angle >= 0; angle-=4) {
      gateServo.write(angle);
      delay(20);
    }
    gateServo.detach(); 
  }  
}

// Function to check all parking slots simultaneously
void checkSlots() {
  // Read all IR sensors at once
  bool isSlot1Occupied = (digitalRead(SLOT1_IR_PIN) == LOW); // Assuming LOW means object detected
  bool isSlot2Occupied = (digitalRead(SLOT2_IR_PIN) == LOW); // Assuming LOW means object detected
  bool isSlot3Occupied = (digitalRead(SLOT3_IR_PIN) == LOW); // Assuming LOW means object detected
  
  updateSlotStatus(1, isSlot1Occupied);  // Update Slot 1 status
  updateSlotStatus(2, isSlot2Occupied);  // Update Slot 2 status
  updateSlotStatus(3, isSlot3Occupied);  // Update Slot 3 status
  Serial.println("--------------"); 
  
  // Print the status on the LCD
  displayStatus(isSlot1Occupied, isSlot2Occupied, isSlot3Occupied);
}

// Helper function to update individual slot status
void updateSlotStatus(int slotNumber, bool isOccupied) {
  String path = "parking_space/slot" + String(slotNumber) + "/status";
  
  if (isOccupied) {
    firebase.setString(path, "Parked");
    Serial.println("Slot " + String(slotNumber) + ": Parked");
  } else {
    firebase.setString(path, "Free");
    Serial.println("Slot " + String(slotNumber) + ": Free");
  }
}

// Function to check the gate IR sensor status
void checkGate() {
  if (digitalRead(GATE_IR_PIN) == LOW) { 
    Serial.println("Gate opened");
    firebase.setString("parking_space/gate/status", "Gate opened");
  } else {
    Serial.println("Gate closed");
    firebase.setString("parking_space/gate/status", "Gate closed");
  }
}

// Function to display parking slot status on the LCD
void displayStatus(bool slot1, bool slot2, bool slot3) {
  lcd.clear();
  
  // Create strings for Free and Parked slots
  String freeSlots = "";
  String parkedSlots = "";

  if (!slot1) freeSlots += "S1 ";
  else parkedSlots += "S1 ";
  
  if (!slot2) freeSlots += "S2 ";
  else parkedSlots += "S2 ";
  
  if (!slot3) freeSlots += "S3 ";
  else parkedSlots += "S3 ";

  // Display the status on two lines
  lcd.setCursor(0, 0);
  lcd.print("Free: " + freeSlots);
  
  lcd.setCursor(0, 1);
  lcd.print("Parked: " + parkedSlots);
}
