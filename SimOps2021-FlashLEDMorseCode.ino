/*Adapted from Programming Arduino by Simon Monk, excersize 5-05. This sketch shows one
 * method for converting text from the serial input to Morse code flashes of an LED. It
 * uses GPIO pin 13 which includes an LED that is built in to the Arduino.
 */

#define dotDelay 200
#define ledPin 13

char* letters[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."          // S-Z
  };

char* numbers[] = {"-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);  
}

void loop() {
  char ch;

  if (Serial.available() > 0)
  {
    ch = Serial.read();
  
    if (ch >= 'a' && ch <= 'z') {
      flashSequence(letters[ch - 'a']);
    }
    else if (ch >= 'A' && ch <= 'Z') {
      flashSequence(letters[ch - 'A']);
    }
    else if (ch >= '0' && ch <= '9') {
      flashSequence(numbers[ch - '0']);
    }
    else if (ch == ' ') {
      delay(dotDelay * 4);  // gap between words  
    }
  }
}

void flashSequence(char* sequence) {

  for (int i = 0; sequence[i] != NULL; i++)
  {
    flashDotOrDash(sequence[i]);
  }
  delay(dotDelay * 3);    // gap between letters
}

void flashDotOrDash(char dotOrDash) {
  digitalWrite(ledPin, HIGH);
  if (dotOrDash == '.') {
    delay(dotDelay);           
  }
  else { 
    // must be a dash 
    delay(dotDelay * 3);           
  }
  digitalWrite(ledPin, LOW);    
  delay(dotDelay); // gap between flashes
}
