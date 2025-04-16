#include <WiFi.h>
#include <esp_now.h>

// Motor driver pins
const int DIR_PIN = 26;  // Direction
const int PWM_PIN = 27;  // Speed

// Structure to receive data (matches transmitter)
typedef struct struct_message {
  char command[10];
} struct_message;
struct_message gestureData;

// Callback for received data
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  memcpy(&gestureData, incomingData, sizeof(gestureData));
  Serial.print("Gesture received: ");
  Serial.println(gestureData.command);

  // Control motor based on gesture
  if (strcmp(gestureData.command, "FORWARD") == 0) {
    digitalWrite(DIR_PIN, HIGH); // Forward direction
    analogWrite(PWM_PIN, 255);   // Full speed
    Serial.println("Motor: Forward");
  } 
  else if (strcmp(gestureData.command, "BACKWARD") == 0) {
    digitalWrite(DIR_PIN, LOW);  // Backward direction
    analogWrite(PWM_PIN, 255);   // Full speed
    Serial.println("Motor: Backward");
  } 
  else { // STOP or any other command
    digitalWrite(DIR_PIN, LOW);
    analogWrite(PWM_PIN, 0);     // Stop motor
    Serial.println("Motor: Stop");
  }
}

void setup() {
  Serial.begin(115200);

  // Set motor pins
  pinMode(DIR_PIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);

  // Set ESP32 as Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Print receiver MAC address (for debugging)
  Serial.print("Receiver MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    while (1);
  }

  // Register callback for receiving data
  esp_now_register_recv_cb(OnDataRecv);

  // Initial state: stop motor
  digitalWrite(DIR_PIN, LOW);
  analogWrite(PWM_PIN, 0);
  Serial.println("Receiver initialized, motor stopped");
}

void loop() {
  // Nothing needed here, everything is handled by the callback
  delay(100); // Small delay to prevent tight loop
}