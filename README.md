# ESP Water Velocity

This project provides a solution for measuring water flow velocity using two ESP32 microcontrollers and HC-SR04 ultrasonic sensors. The system calculates water flow velocity by measuring the time it takes for water to reach two sensor points and comparing it with expected velocity calculations using Manning's formula for partially filled pipes. It utilizes ESP-NOW for wireless communication between the two ESP32 boards. Additionally, a web server interface allows users to monitor sensor status, calibrate settings, and configure warning thresholds. A GSM module is also integrated to send SMS alerts when water levels reach critical thresholds.

## Features

- **Real-time Water Flow Velocity Measurement**: Calculates velocity based on the time difference between two ultrasonic sensors using Manning's formula.
- **Water Level Detection**: Monitors and logs water level changes in real time.
- **Wireless Communication**: Uses ESP-NOW for fast and reliable data transmission between the two ESP32 boards.
- **Web Server Interface**: Provides a UI for users to monitor sensor readings and configure settings.
- **Customizable Calibration**: Users can adjust pipe depth, slope, and thresholds via the web interface.
- **GSM Alerts**: Sends SMS warnings when water reaches defined thresholds.
- **I2C LCD Display**: The main ESP32 has an I2C LCD that displays real-time water velocity measurements.

## Hardware Requirements

- **2x ESP32 Development Boards**: One acts as the transmitter and the other as the receiver.
- **2x HC-SR04 Ultrasonic Sensors**: Measures water level and time of arrival for flow velocity calculation.
- **1x GSM Module (SIM800L or similar)**: Sends SMS alerts when water levels reach warning or critical thresholds.
- **1x I2C LCD Display**: Displays real-time water velocity data.
- **Connecting Wires**: For interfacing the sensors with the ESP32.

## Circuit Diagram

### Remote ESP32

- **HC-SR04 Ultrasonic Sensor**:
  - **VCC**: Connect to 5V on the ESP32.
  - **GND**: Connect to GND on the ESP32.
  - **Trigger Pin**: Connect to GPIO 5 on the ESP32.
  - **Echo Pin**: Connect to GPIO 18 on the ESP32.

### Main ESP32

- **HC-SR04 Ultrasonic Sensor**:

  - **VCC**: Connect to 5V on the ESP32.
  - **GND**: Connect to GND on the ESP32.
  - **Trigger Pin**: Connect to GPIO 5 on the ESP32.
  - **Echo Pin**: Connect to GPIO 18 on the ESP32.

- **GSM Module (SIM800L) - Main ESP32**:

  - **VCC**: Connect to 4.2V–5V power source.
  - **GND**: Connect to GND on the ESP32.
  - **TX**: Connect to GPIO 27 on the ESP32.
  - **RX**: Connect to GPIO 26 on the ESP32.

- **I2C LCD Display - Main ESP32**:

  - **SDA**: Connect to GPIO 21 on the ESP32.
  - **SCL**: Connect to GPIO 22 on the ESP32.

## User Guide

### Calibration Guide

To calibrate the remote ESP32:

1. Connect to the **ESP32_Remote** Wi-Fi network.
2. Use the password: `00000001`.
3. Open a web browser and enter the device's IP address.
   ![Remote Web Server](images\remote_web_server.jpg)
4. **Pipe Depth**: Use the calibrate button to automatically callibrate the pipe depth.
5. **Water Level**: Displays current water level at remote sensor
6. **ESP Main Status**: Indicates if the main ESP32 is active

For the main ESP32:

1. Connect to the **ESP32_Main** Wi-Fi network.
2. Use the password: `00000000`.
3. Open a web browser and enter `192.168.4.1` in the address bar.
   ![Main Web Server](images\main_web_server.jpg)
4. **Pipe Depth**: Use the calibrate button to automatically callibrate the pipe depth.
5. **ESP Remote Status**: Indicates if the remote ESP32 is active
6. **Water Level**: Displays current water level in `mm`
7. **Velocity**: Displays calculated water velocity in `m/s` (also shown on the I2C LCD display)
8. **ESP Remote Water Level**: Shows water level measured by the remote ESP32
9. **Flowing Status**: Indicates if water is detected as flowing
10. **Time Taken**: Measures time taken for water to travel between sensors
11. **Slope Configuration**: Adjustable value (e.g., `0.0666667`)
12. **Phone Numbers for Warnings**: Users can input numbers (e.g., `+639950281057, +63992436721`)
13. **Calibration Options**: Users can calibrate the pipe depth and flow measurement settings.

## GSM Alert System

The system sends SMS alerts when water reaches certain levels:

- **Warning Threshold**: A message is sent to the configured phone numbers.
- **Critical Threshold**: A more urgent alert is sent for immediate action.

Ensure a SIM card is inserted into the GSM module and that it has sufficient balance for SMS messaging.

## Power Considerations

The ESP32 and peripherals require a stable power source:

- **ESP32 Average Consumption**: \~100-200mA in active mode.
- **HC-SR04 Consumption**: \~15mA.
- **SIM800L Consumption**: Up to 2A during transmission.
- **I2C LCD Display Consumption**: \~20mA.
- **Recommended Power Source**: A 5V, 3A power adapter or battery system.
- **Solar Option**:
  - A **10W solar panel** (5V, 2A output) with a **lithium battery pack (e.g., 3.7V 6000mAh)** and a **boost converter** can provide reliable power.
  - Use a **TP4056 charging module** to regulate charging and prevent over-discharge.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
