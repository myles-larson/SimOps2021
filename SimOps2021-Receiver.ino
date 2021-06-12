/*Receiver (valve) side Arduino. Includes RadioHead NRF24L01 library, valve controls, and LED controls.
 * This code contains multiple methods of doing the same task, such as flashing an LED, to 
 * show more ways of controlling the Arduino.
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

/*These constants and variables that come before the setup function are called global.
 * They are available to any function in the sketch as opposed to variables declared
 * in a function which are only available to that function.
 */
// Designate the pin that will open our valve. Using #define can save memory and create a
// constant association of openV with the value 5.
#define openV 5

// Designate the pin for our status LED. Another way to designate a constant number.
const int statusLed = 9;

// Create our time out variable for closing the valve as an integer.
int timeOut = 0;

// Create our time check variable for holding the valve open as a long integer.
unsigned long howLongSince = 0;

// How much time do we want the valve open in milliseconds?
const int openTime = 30000;

// setup() is part of every Arduino sketch.
void setup() {
  
  // Set up our pins
  pinMode(openV, OUTPUT);
  pinMode(statusLed, OUTPUT);

  // Prepare for serial output to the computer's serial monitor.
  Serial.begin(9600);

  // Set up our wireless connection and check each step for failure.
  if (!radi0.init())
  {
    // Send the text "init fail" to the computer's serial monitor.
    Serial.println("init failed");
    // Show a set of flashes if initialization fails using the statusBlink function defined below.
    statusBlink(2, 1000);
  }

  // If init works default channel, data rate, and transmit power are set.
  // Defaults: 2.402 GHz (channel 2), 2Mbps, 0dBm

  // Set channel to 111. Channels must match between the two radios.
  if (!radi0.setChannel(111))
  {
    Serial.println("setChannel failed");
    // Show a set of flashes if setChannel fails by manually writing the blinks (functions save time).
    digitalWrite(statusLed, HIGH);
    delay(100);
    digitalWrite(statusLed, LOW);
    delay(1000);
    digitalWrite(statusLed, HIGH);
    delay(100);
    digitalWrite(statusLed, LOW);
    delay(1000);
    digitalWrite(statusLed, HIGH);
    delay(100);
    digitalWrite(statusLed, LOW);
  }

  /* //Uncomment this to set the data rate and transmission power
  if (!radi0.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");*/

    Serial.println("Success: radio is up. Receiver is ready.");
}

// loop() is part of every Arduino sketch.
void loop() {

  /*Set up our variable for tracking time. This will be done using the millis()
   * function instead of the delay() function as seen in the StatBlink function below.
   * The reason for using millis() here is that delay() will halt all other processing
   * while it runs but millis() lets our program continue. This means we can receive
   * another wireless message while our valve is open which just increases the time the valve
   * stays open. If we used delay() to hold the valve open, any additional wireless
   * messages would be ignored.
   */
  unsigned long currentRunTime = millis();
  
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
      statusBlink(6, 100);
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
        // Set our time to check against.
        howLongSince = currentRunTime;
      }
    }
  }

/* We will check if our valve is open (pin set to HIGH), and if we have waited long enough.
 * PORTD is the data register for digital pins # 0-7, PORTB is for digital pins #8-13 (PORTC is for analog input pins).
 * In this case we are using bitRead to check the current HIGH or LOW state of the openV pin, and if it is 1,
 * meaning HIGH, the valve is open.
 */

  if (bitRead(PORTD, openV) == 1 && howLongSince + openTime < currentRunTime)
  {
    // Close the valve.
    digitalWrite(openV, LOW);
    Serial.println("Close valve.");
    statusBlink(3,10);
  }
}

/* This is a function outside of our setup and loop functions. We can call this function by name
 *  and pass it two integers: blinkCount and blinkSpeed. Functions like this let us reuse code
 *  quickly and easily elsewhere in our sketch.
 */
void statusBlink(int blinkCount, int blinkSpeed)
{
  // While the condition blinkCount is greater than or equal to 0 do something
  while (blinkCount >= 0)
  {
    // Turn on the LED
    digitalWrite(statusLed, HIGH);
    // Wait for an amount of milliseconds
    delay(blinkSpeed);
    // Turn the LED off
    digitalWrite(statusLed, LOW);
    // Wait again
    delay(blinkSpeed);
    // Decrement the counter (subtract one)
    blinkCount--;
  }

}
