#include "BluetoothSerial.h"

// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` and enable it
#endif

// Bluetooth Serial object
BluetoothSerial SerialBT;

// GPIO where LED is connected to
const int ledPin =  4;
// Handle received and sent messages
String message = "";

// Variable to track the LED state
bool ledState = false;

// Variable to track connection status
bool isConnected = false;

void setup()
{
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW); // Initialize LED to OFF
    Serial.begin(115200);
    
    // Bluetooth device name
    SerialBT.begin("ESP32"); // Your mobile phone should detect this name
    Serial.println("The device started, now you can pair it with Bluetooth!");
}

void loop() 
{
    // Check for a new connection
    if (SerialBT.hasClient() && !isConnected)
    {
        isConnected = true;
        // Send the current LED state when a new connection is established
        if (ledState)
        {
            SerialBT.println("The LED is ON");
        }
        else
        {
            SerialBT.println("The LED is OFF");
        }
    }

    // Check if the connection is lost
    if (!SerialBT.hasClient() && isConnected)
    {
        isConnected = false;
        Serial.println("Bluetooth device disconnected");
    }

    // Read from the serial monitor
    if (Serial.available())
    {
        char inputChar = Serial.read();
        if (inputChar != '\n')
        {
            message += String(inputChar);
        }
        else
        {
            message = "";
        }
    }
    
    // Read from Bluetooth
    if (SerialBT.available())
    {
        char inputChar = SerialBT.read();
        if (inputChar != '\n')
        {
            message += String(inputChar);
        }
        else
        {
            message = "";
        }
        Serial.write(inputChar);  
    }
    
    // Handle "led_on" and "led_off" messages
    if (message == "led_on")
    {
        digitalWrite(ledPin, HIGH);
        ledState = true; // Update LED state
        SerialBT.println("The LED is ON");
    }
    else if (message == "led_off")
    {
        digitalWrite(ledPin, LOW);
        ledState = false; // Update LED state
        SerialBT.println("The LED is OFF");
    }
    
    delay(20);
}
