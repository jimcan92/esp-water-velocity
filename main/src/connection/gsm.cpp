#include "gsm.h"
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

TaskHandle_t gsmTaskHandle = NULL;
QueueHandle_t smsQueue;

bool gsmInitialized = false;

struct SMSMessage
{
    String phoneNumber;
    String message;
};

void gsmTask(void *pvParameters);
void splitNumbers(String numbers, String output[MAX_NUMBERS], int &count);

GSM::GSM() : sim800l(2) {}

void GSM::init()
{
    debugger.println("Initializing SIM800L...");

    sim800l.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
    delay(2000); // Wait for SIM800L to stabilize

    debugger.println("Sending AT commands to check module response...");

    if (sendATCommand("AT"))
    {
        debugger.println("SIM800L is responding.");
    }
    else
    {
        debugger.println("SIM800L did not respond. Check wiring and power.");
        return;
    }

    debugger.println("Checking signal quality...");
    sendATCommand("AT+CSQ"); // Check signal strength

    debugger.println("Checking network registration...");
    sendATCommand("AT+CREG?"); // Check if registered on network

    debugger.println("GSM Module initialized successfully.");

    smsQueue = xQueueCreate(QUEUE_SIZE, sizeof(SMSMessage));
    if (smsQueue == NULL)
    {
        debugger.println("Failed to create SMS queue!");
        return;
    }

    gsmInitialized = true;

    // Start GSM task on Core 1
    xTaskCreatePinnedToCore(gsmTask, "GSM Task", 4096, this, 1, &gsmTaskHandle, 1);
}

// Function to queue SMS for sending
bool GSM::queueSMS(const String &message, const String &phoneNumber)
{
    if (!gsmInitialized)
        return false;

    if (smsQueue == nullptr)
    {
        debugger.println("SMS Queue is not initialized!");
        return false;
    }

    SMSMessage sms;
    sms.message = message;
    sms.phoneNumber = phoneNumber;

    if (xQueueSend(smsQueue, &sms, pdMS_TO_TICKS(500)) == pdTRUE)
    {
        debugger.print("SMS Queued: ");
        debugger.println(phoneNumber);
        return true;
    }
    else
    {
        debugger.println("SMS Queue is full! Message not sent.");
        return false;
    }
}

// Function to queue SMS to multiple recipients
void GSM::queueSMSToAll(String message)
{
    String phoneArrays[MAX_NUMBERS];
    int count = 0;
    splitNumbers(phoneNumbers, phoneArrays, count);

    for (int i = 0; i < count; i++)
    {
        queueSMS(message, phoneArrays[i]);
    }
}

// Send AT command without blocking
bool GSM::sendATCommand(const String &command)
{
    debugger.print("Sending: ");
    debugger.println(command);

    sim800l.println(command);
    delay(500); // Short delay for response

    String response = "";
    unsigned long startTime = millis();
    while (millis() - startTime < 2000) // Wait up to 2 seconds for response
    {
        if (sim800l.available())
        {
            char c = sim800l.read();
            response += c;
        }
    }

    debugger.print("Response: ");
    debugger.println(response);

    return response.indexOf("OK") != -1; // Return true if "OK" is found
}

bool GSM::waitForNetworkRegistration()
{
    unsigned long startTime = millis();
    while (millis() - startTime < 10000) // Wait up to 10 seconds
    {
        if (sendATCommand("AT+CREG?"))
        {
            String response = "";
            while (sim800l.available())
            {
                response += (char)sim800l.read();
            }

            if (response.indexOf("+CREG: 0,1") != -1 || response.indexOf("+CREG: 0,5") != -1)
            {
                debugger.println("✅ SIM is registered to the network.");
                return true;
            }
        }

        debugger.println("📡 Waiting for network registration...");
        vTaskDelay(pdMS_TO_TICKS(2000)); // Wait 2 seconds before retrying
    }

    debugger.println("❌ SIM registration failed. Check SIM and signal.");
    return false;
}

bool GSM::sendSMS(const String &message, const String &phoneNumber)
{
    debugger.println("Sending SMS to: " + phoneNumber);

    if (!waitForNetworkRegistration())
    {
        return false;
    }

    // Set SMS text mode
    if (!sendATCommand("AT+CMGF=1"))
    {
        debugger.println("Failed to set SMS text mode.");
        return false;
    }
    vTaskDelay(pdMS_TO_TICKS(500)); // Non-blocking delay

    // Start SMS sending command
    sim800l.print("AT+CMGS=\"");
    sim800l.print(phoneNumber);
    sim800l.println("\"");
    vTaskDelay(pdMS_TO_TICKS(500));

    // Send message
    sim800l.print(message);
    vTaskDelay(pdMS_TO_TICKS(500));

    // Send Ctrl+Z (ASCII 26) to finalize message
    sim800l.write(26);

    // Wait for response (up to 5 seconds)
    String response = "";
    uint32_t startTime = millis();
    while (millis() - startTime < 5000)
    {
        while (sim800l.available())
        {
            char c = sim800l.read();
            response += c;
        }

        if (response.indexOf("+CMGS:") != -1)
        {
            debugger.println("SMS sent successfully.");
            return true;
        }
        else if (response.indexOf("ERROR") != -1)
        {
            debugger.println("SMS sending failed!");
            return false;
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Small delay to prevent CPU overuse
    }

    debugger.println("SMS sending timed out.");
    return false;
}

void gsmTask(void *pvParameters)
{
    GSM *gsm = (GSM *)pvParameters;
    SMSMessage sms;

    while (true)
    {
        // Check for SMS messages in queue
        if (xQueueReceive(smsQueue, &sms, 0) == pdTRUE)
        {
            bool success = gsm->sendSMS(sms.message, sms.phoneNumber);

            if (!success)
            {
                debugger.println("Retrying SMS in 10 seconds...");
                vTaskDelay(pdMS_TO_TICKS(10000));               // Wait 10 sec before retrying
                xQueueSend(smsQueue, &sms, pdMS_TO_TICKS(100)); // Re-add SMS to queue
            }
            // Send Ctrl+Z
        }

        // Sleep briefly to allow other tasks to run
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void splitNumbers(String numbers, String output[MAX_NUMBERS], int &count)
{
    count = 0;
    while (numbers.length() > 0 && count < MAX_NUMBERS)
    {
        int commaIndex = numbers.indexOf(",");
        if (commaIndex == -1)
        {
            output[count++] = numbers; // Last number
            break;
        }
        output[count++] = numbers.substring(0, commaIndex);
        numbers = numbers.substring(commaIndex + 1);
    }
}