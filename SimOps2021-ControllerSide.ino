/*Controller (button) side Arduino. Includes RadioHead NRF24L01 library, button, and LED controls.
 * This code contains multiple methods of doing the same task, such as flashing an LED, to
 * show more ways of controlling the Arduino.
 */

/*How our radio pins are wired to our Arduino pins:
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
// Set which pin # we will read for our button press. Using #define can save memory and create a
// constant association of the word button with the value 2.
#define button 2

// Set which pin # we will use for indicator LED. Another way to designate a constant number.
const int led = 3;

void setup() {

  // Set up our pins
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  
  // Prepare for serial output to the computer's serial monitor
  Serial.begin(9600);

  // Set up our wireless connection and check each step for failure
  if (!radi0.init())
  {
    // Send the text "init fail" to the computer's serial monitor
    Serial.println("init fail");
    //Show a set of flashes if init fails with manually written HIGH and LOW
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(100);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
  }
    
  // If init works default channel, data rate, and transmit power are set
  // Defaults: 2.402 GHz (channel 2), 2Mbps, 0dBm

  //Set the channel to 111. Channels must match between radios.
  if (!radi0.setChannel(111))
  {
    Serial.println("SetChannel fail");
    //Show a set of flashes if setChannel fails using the statusBlink function below (functions save time)
    statusBlink(2, 50);
    statusBlink(2, 100);
  }

  /* // Uncomment to set data rate and transmission power.
  if (!radi0.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("serRF fail");*/

  Serial.println("Success: radio is up. Controller ready.");
  //sendMess("Radio up. Controller up.");
}

void loop() {
  // Watch for the button to be pressed, then send a wireless signal to the Receiver
  if (digitalRead(button) == HIGH)
  {   
    // Send our message
    sendMess("100");
    // Flash the LED to show we sent the message
    statusBlink(6, 50);
  }
  //Wait here for a moment and then start again
  delay(100);

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
    // Turn the LED on
    digitalWrite(led, HIGH);
    // Wait for an amount of milliseconds
    delay(blinkSpeed);
    // Turn the LED off
    digitalWrite(led, LOW);
    // Wait again
    delay(blinkSpeed);
    // Decrement our counter (subtract one)
    blinkCount--;
  }

}

/* This functions sends a message out on the radio. It accepts text. It takes the text passed to it
 *  as a constant to prevent changes to the message we are sending on the radio. The * is called a
 *  pointer which means we are referencing the text but not actually working with it directly. This
 *  differs from a variable such as an int (positive or negative integer) where we are working directly
 *  with the value of the variable.
 */
void sendMess(const char* textToSend)
{
  // How many characters are in our message
  int sizeTextToSend = strlen(textToSend);

  // Create an array (think of it like a list of things) with a size of how many characters are in textToSend
  uint8_t ourMessage[sizeTextToSend];
  
  // These can be usefule for debugging or just to see what the code is doing.
  Serial.println("Num of char: ");
  Serial.println(sizeTextToSend);
  Serial.println(" Message size: ");
  Serial.println(sizeof(ourMessage));
  
  // Create a loop to add each character of textToSend to the array called ourMessage
  for (int i=0; i <= sizeTextToSend; i++)
  { 
    ourMessage[i] = textToSend[i];
    
    // These can be usefule for debugging or just to see what the code is doing.
    Serial.println(i);
    Serial.println(ourMessage[i]);
    Serial.println(textToSend[i]);
  } 
  // Send our message
  radi0.send(ourMessage, sizeof(ourMessage));
  // Wait for it to finish sending before moving on. 
  radi0.waitPacketSent();
}
