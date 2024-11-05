# [ParkBuddy App](https://darishsfit.github.io/parkbuddy.github.io/) 

### [ParkBuddy App Source Repository](https://okay-joby-danielthatu47-210548f0.koyeb.app/)

#### This project uses an ESP8266 microcontroller to monitor a parking spot's occupancy status and control a gate using a servo motor. The data is sent to Firebase Realtime Database, and the system includes indicators (LEDs) to show the status of the parking spot.

## Libraries

- `ESP8266WiFi.h` - For connecting the ESP8266 to Wi-Fi.
- `ESP8266Firebase.h` - For interacting with the Firebase Realtime Database.
- `Servo.h` - For controlling the servo motor.

## Key Components

- **ESP8266 Microcontroller**
- **IR Sensors**
- **Servo Motor (SG90)**
- **LED Lights**
- **Firebase Realtime Database**


```cpp
#include <ESP8266WiFi.h>              // Library for ESP8266 Wi-Fi functionality.
#include <ESP8266Firebase.h>          // Library for Firebase Realtime Database integration.
#include <Servo.h>                    // Library for controlling the servo motor.
```

- **`ESP8266WiFi.h`:** Manages Wi-Fi connections.
- **`ESP8266Firebase.h`:** Integrates with Firebase.
- **`Servo.h`:** Controls the servo motor.

### Wi-Fi and Firebase Configuration

```cpp
#define _SSID "your_hotspot_ssid"              // Wi-Fi network SSID.
#define _PASSWORD "password"          // Wi-Fi network password.
#define REFERENCE_URL "https://your-firebase-db-default-rtdb.firebaseio.com"  // Firebase Realtime Database URL.

Firebase firebase(REFERENCE_URL);      // Initialize Firebase with the database URL.
Servo gateServo;                      // Create a Servo object for the gate.
```

- **`_SSID` and `_PASSWORD`:** Your Wi-Fi credentials.
- **`REFERENCE_URL`:** URL for your Firebase Realtime Database.
- **`Firebase firebase(REFERENCE_URL);`:** Initializes Firebase.
- **`Servo gateServo;`:** Creates a servo object for gate control.

### Setup Function

- **`Serial.begin(115200);`:** Initializes serial communication.
- **`WiFi.begin(_SSID, _PASSWORD);`:** Connects to the Wi-Fi network.
- **`WiFi.status()`:** Checks the connection status.
- **`gateServo.attach(D0);`:** Connects the servo motor to pin D0.
- **`firebase.setString()`, `firebase.setInt()`, `firebase.setFloat()`:** Example Firebase operations to set data.
  
## Important Notes

- Ensure your Firebase project’s rules are set to allow read and write operations. 
- Modify the Wi-Fi and Firebase credentials with your own details.

For more details, visit the [ESP8266Firebase GitHub repository](https://github.com/Rupakpoddar/ESP8266Firebase).

----

# NodeMCU (ESP8266)

**How does it work?**
The NodeMCU is a low-cost microcontroller that integrates the ESP8266 chip. It operates through Wi-Fi, allowing you to easily create IoT projects. It supports GPIO (General Purpose Input Output) pins for sensors, actuators, and communication protocols like I2C and SPI.

**Technical Specifications**
 - Pin count: 11 GPIO pins
 - Clock speed: 80 MHz (can go up to 160 MHz)
 - Memory: 64 KB instruction RAM, 96 KB data RAM, and 4 MB flash storage
 - Operating voltage: 3.3V
 - Wi-Fi: 802.11 b/g/n support with WPA/WPA2 security
 - ADC: 10-bit Analog to Digital Converter (ADC) with one input pin (A0)
 - Communication interfaces: UART, SPI, I2C

**Specific Features**
 - Built-in Wi-Fi connectivity
 - Power-saving modes for efficient energy consumption (deep sleep mode)
 - Supports Lua and Arduino IDE for easy programming

**Common Features**
 - USB to serial interface
 - Onboard LEDs for power and status indication
 - Integrated voltage regulator (5V to 3.3V)

----

# IR Sensor

**How does it detect object?** <br>
The IR sensor uses an infrared LED to emit light, and a photodiode to detect the reflection of that light. When an object is in proximity, the reflected light is detected by the photodiode, indicating the presence of the object.

**How does it adjust range?** <br>
The range can be adjusted by turning the onboard potentiometer. Rotating the trimmer adjusts the sensitivity of the sensor’s digital output, allowing you to detect objects at different distances.

**Where can you use it?** <br>
You can use this sensor for sensing black and white lines, suitable for making optical pattern projects and line following robots. Also, you can use this sensor to sense frequency of object. <br>
Note: this sensor only works in DIGITAL mode. You can connect the digital output (DO) to sense. Adjust the potentiometer (a.k.a. trimmer) to set the digital output (DO) sensitivity.

**Specific Features**
- Comes with a pair of 5mm infrared LED (emitter) and photodiode (receiver) work as reflect optical sensor
- Wavelength : 940 nm
- Output type : Digital (HIGH or LOW)
- Model code : FC-51

**Common Features**
- Logical IC : LM393
- Operating voltage : 3.3 – 5V
- Output current : ?15 mA
- Adjustable sensitivity via potentiometer
- Comes with LED indicators for POWER and OUTPUT
- Fixed bolt holes for easy installation

----

# Servo Motor (SG90)
**How does it spin?** <br>
The SG90 servo motor rotates between 0 to 180 degrees based on the PWM (Pulse Width Modulation) signal it receives. It can hold its position once set, making it ideal for tasks like controlling a gate or robotic arm.

**How does it receive input?** <br>
The servo receives input via the PWM signal sent to it from a microcontroller like the NodeMCU. The width of the pulse determines the angle of rotation. For instance, a 1 ms pulse moves the servo to 0°, while a 2 ms pulse moves it to 180°.

**Specific Features** <br>
- Torque: 1.8 kg-cm at 4.8V
- Speed: 0.1 sec/60° at 4.8V
- Rotation range: 0° to 180°
- Operating voltage: 4.8V to 6V

**Common Features** <br>
- Dimensions: 22.2 x 11.8 x 31 mm
- Weight: 9g
- Includes mounting hardware and servo horns
- Gear type: Plastic gears for lightweight applications

----

