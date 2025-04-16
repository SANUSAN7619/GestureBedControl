#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <esp_now.h>

// MPU6050 object
MPU6050 mpu;

// Flex sensor pins
const int FLEX_PIN1 = 34; // Flex sensor 1
const int FLEX_PIN2 = 35; // Flex sensor 2

// Thresholds (adjusted based on your observations)
const int FLEX_THRESHOLD = 800;  // Lowered to detect bends around 1000
const float TILT_THRESHOLD = 20.0; // Tilt angle in degrees

// MAC Address of the receiver ESP32 (replace with your receiver's actual MAC)
uint8_t receiverMacAddress[] = {0xD4, 0x8A, 0xFC, 0xAA, 0x17, 0x94}; // Example: {0x24, 0x6F, 0x28, 0xA1, 0xB2, 0xC3}

// Structure to send data (matches receiver)
typedef struct struct_message {
  char command[10];
} struct_message;
struct_message gestureData;

// Callback for ESP-NOW send status
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  // Set ESP32 as Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Print transmitter MAC address (for debugging)
  Serial.print("Transmitter MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    while (1);
  }

  // Register send callback
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;  // Default channel
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    while (1);
  }
}

void loop() {
  // Read flex sensors
  int flex1Value = analogRead(FLEX_PIN1);
  int flex2Value = analogRead(FLEX_PIN2);

  // Read MPU6050
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Calculate roll (left/right tilt) and pitch (forward/back tilt) in degrees
  float roll = atan2(ay, az) * 180 / PI;   // Roll: Not used here, kept for reference
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / PI; // Pitch: Used for left/right tilt

  // Debug output to check orientation
  Serial.print("Flex1: ");
  Serial.print(flex1Value);
  Serial.print(" | Flex2: ");
  Serial.print(flex2Value);
  Serial.print(" | Roll: ");
  Serial.print(roll);
  Serial.print(" | Pitch: ");
  Serial.println(pitch);

  // Gesture detection
  if (flex1Value > FLEX_THRESHOLD && pitch > TILT_THRESHOLD) { // Tilt right + Flex 1 bend
    strcpy(gestureData.command, "FORWARD");
    Serial.println("Gesture: Move Forward");
  } 
  else if (flex2Value > FLEX_THRESHOLD && pitch < -TILT_THRESHOLD) { // Tilt left + Flex 2 bend
    strcpy(gestureData.command, "BACKWARD");
    Serial.println("Gesture: Move Backward");
  } 
  else {
    strcpy(gestureData.command, "STOP");
    Serial.println("Gesture: Stop");
  }

  // Send gesture data via ESP-NOW
  esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *) &gestureData, sizeof(gestureData));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  delay(100); // Adjust delay as needed
}