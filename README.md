# 🍓 Gesture Controlled Bed for Assisted Comfort

<img src="https://github.com/SANUSAN7619/GestureBedControl/blob/master/fruity.png" width="100%" height="300px"/>

This project aims to improve comfort and independence for individuals with physical mobility challenges by allowing them to control bed adjustments using hand gestures.

## 🍓🍰 Project Overview

The system uses:
- **MPU6050** to detect wrist tilts (left/right)
- **Flex sensors** to detect finger bending
- **ESP32** microcontrollers for gesture processing and wireless data transmission
- **MDD10A motor driver** to control the bed's movement (forward/backward)

##  🍓🍰How It Works

1. **User wears a smart glove** fitted with sensors.
2. A gesture is performed:
   - Bend + right tilt → Move forward
   - Bend + left tilt → Move backward
3. The transmitter ESP32 processes and sends this gesture via **ESP-NOW** to another ESP32.
4. The receiver controls a **DC motor** through the **MDD10A driver**, adjusting the bed.

## 🍓🍰 Features

- Wireless communication between ESP32 boards
- Real-time bed movement
- Designed with simplicity, safety, and comfort in mind
- Rechargeable battery-powered glove setup

## 🍓🍰 Hardware Used

- ESP32 Dev Module (x2)
- MPU6050 sensor
- Flex sensors (x2)
- MDD10A Motor Driver
- DC motor
- 9V battery and LM7805 regulator
- Glove and bed prototype

## 🍓🍰 Applications

- Patient care and elderly support
- Hospital beds
- Smart healthcare systems
- Home automation for differently abled users

## 🍓🍰 Developed By

Sanjana Suresh (team member)

Muthoot Institute of Technology and Science  
Third year B.Tech – Electronics & Communication  
2025

---

