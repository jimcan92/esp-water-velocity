#ifndef GSM_H
#define GSM_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>

// #include "../display/display.h"
#include "../display/debug.h"
#include "../utils/structs.h"

// extern Display lcd;
extern Debug debugger;
extern String phoneNumbers;

#define RX_PIN 16      // ESP32 RX2 -> SIM800L TXD
#define TX_PIN 17      // ESP32 TX2 -> SIM800L RXD
#define BAUD_RATE 9600 // Baud rate for the SIM800L module

#define QUEUE_SIZE 10
#define MAX_NUMBERS 10

class GSM
{
public:
    GSM();
    void init();
    bool queueSMS(const String &message, const String &phoneNumber);
    void queueSMSToAll(String message);
    bool sendSMS(const String &message, const String &phoneNumber);

private:
    bool sendATCommand(const String &command);
    bool waitForNetworkRegistration();
    HardwareSerial sim800l;
};

#endif