/*Receiver (valve) side Arduino. Includes RadioHead NRF24L01 library, valve controls, and LED controls.
 * This code is a simple listening radio with some LED flashes to indicate the status of the system.
 */

/*How our radio pins are wired to our Arduino pins as defined in the RadioHead library:
 * NRF24L01 > Arduino   (Description)
 * GND    >   GND       (Ground)
 * VCC    >   3.3V      (3.3 Volts)
 * CE     >   8         (Input to control data transmission and reception)
 * CSN    >   10         (Chip Select Not - enables the SPI bus when HIGH)
 * SCK    >   13        (Serial Clock - synchronizes data transmission)
 * MOSI   >   11        (Master Out Slave In - Master line for sending data to the slave)
 * MISO   >   12        (Master In Slave Out - Slave line for sending data to the master)
 * IRQ    >   Not Used  (Interrupt request, not used)
 */


// Will use SPI (Serial Peripheral Interface) to communicate between Arduino and NRF24L01
#include <SPI.h>

// Add the library for the NRF24L01
#include <RH_NRF24.h>

// Singleton instance (meaning restricted to one instance) of the radio driver
// We will be able to use member functions of the RH_NRF24 library using radi0.function()
RH_NRF24 radi0;

// Designate the pin that will open our valve. Using #define can save memory and create a
// constant association of openV with the value 5.
#define openV 5

// setup() is part of every Arduino sketch.
void setup() {
  
  // Set up our pins
  pinMode(openV, OUTPUT);

  // Prepare for serial output to the computer's serial monitor.
  Serial.begin(9600);

  // Set up our wireless connection and check each step for failure.
  if (!radi0.init())
  {
    // Send the text "init fail" to the computer's serial monitor.
    Serial.println("init failed");
  }

  // If init works default channel, data rate, and transmit power are set.
  // Defaults: 2.402 GHz (channel 2), 2Mbps, 0dBm

  // Set channel to 111. Channels must match between the two radios.
  if (!radi0.setChannel(111))
  {
    Serial.println("setChannel failed");
  }

  Serial.println("Success: radio is up. Receiver is ready.");
}

// loop() is part of every Arduino sketch.
void loop() {
  
  // If our wireless radio has data available, process that data.
  if (radi0.available())
  {
    // Create our buffer as an array of type unsigned integer with a
    // size of RH_NRF24_MAX_MESSAGE_LEN (a constant defined in the NRF24L01 library).
    uint8_t buffy[RH_NRF24_MAX_MESSAGE_LEN];
    // Create a variable as type unsigned integer with size of one byte and store the size of our buffer in it.
    uint8_t len = sizeof(buffy);
    
    // Read the message data.
    if (radi0.recv(buffy, &len))
    {
      // Some output, useful for debugging.
      Serial.print("got request: ");
      // Show a the message we received
      Serial.println((char*)buffy);

      // Convert the message from characters to numbers.
      int sig = atoi((const char*) buffy);
      // Check if we got the right number to open the valve.
      if (sig == 100)
      {
        // We got the right number so now we'll do something:
        // Open the valve.
        digitalWrite(openV, HIGH);
        Serial.println("Open valve.");
        // Pause here and don't do anything else for 30 seconds.
        delay(30000);
      }
    }
  }
}
