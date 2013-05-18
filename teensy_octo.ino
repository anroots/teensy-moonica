/**
 * Teensy Octo v1.0
 *
 * This is the C / Arduino firmware for the Teensy Octo,
 * a custom-built USB peripheral with eight buttons, two
 * PWM RGB LEDs and a Piezo buzzer.
 * 
 * The microcontroller in use is Teensy 2.0 - http://www.pjrc.com/teensy
 * 
 * Created as a birthday present to Waher (http://waher.net/about-waher-net)
 * 
 * @author David <david@sqroot.eu> 2013
 * 
 * Useful links:
 * 
 * http://www.pjrc.com/teensy/td_libs_Bounce.html - About button bouncing
 * http://www.pjrc.com/teensy/pinout.html - Teensy pinout diagram
 * http://www.pjrc.com/teensy/td_usage.html - Reprogramming
 * https://github.com/anroots/teensy-octo - Repository for the firmware / wrapper
 * https://www.circuitlab.com/circuit/s6dr46/octo/ - Circuit diagram
 **/

#include <Bounce.h>

#define DEBOUNCE 10  // Number of ms to debounce
#define NUMBER_OF_BUTTONS 8
#define NUMBER_OF_LEDS 6 // 2 RGB LEDs, 3 * 2

// Define digital pins for the buttons
const byte BUTTON_PINS[] = { 0, 1, 2, 3, 7, 8, 20, 21 };

// Define PWM pins for the RGB LED
// Order: LED0-R, LED0-G, LED0-B; LED1-R, LED1-G, LED1-B
const byte LED_PINS[] = { 9, 5, 10, 14, 12, 15 };

// The Piezo buzzer sits in this PWM pin
const byte BUZZER_PIN = 4;

// Initialize button objects
// We access the buttons through these Bounce objects
// instead of directly reading them (see: debouncing)
Bounce buttons[NUMBER_OF_BUTTONS] = {
  Bounce( BUTTON_PINS[0], DEBOUNCE),
  Bounce( BUTTON_PINS[1], DEBOUNCE),
  Bounce( BUTTON_PINS[2], DEBOUNCE),
  Bounce( BUTTON_PINS[3], DEBOUNCE),
  Bounce( BUTTON_PINS[4], DEBOUNCE),
  Bounce( BUTTON_PINS[5], DEBOUNCE),
  Bounce( BUTTON_PINS[6], DEBOUNCE),
  Bounce( BUTTON_PINS[7], DEBOUNCE)
  };

  /**
   * Setup function - run once on power-on
   **/
  void setup() {

    // The baud rate is ignored and communication always occurs at full USB speed.
    // http://www.pjrc.com/teensy/td_serial.html
    Serial.begin(9600);

    // Initialize pushbutton pins as inputs with the internal pullup resistor enabled.
    // http://www.pjrc.com/teensy/tutorial3.html
    for (byte i = 0; i < NUMBER_OF_BUTTONS; i++) {
      pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    }

    // Initialize LED pins as output
    for (byte i = 0; i < NUMBER_OF_LEDS; i++) {
      pinMode(LED_PINS[i], OUTPUT);
      analogWrite(LED_PINS[i], 255);
    }
  }

/**
 * Main loop - the program keeps repeating this forever, as long as there is power
 **/
void loop()                     
{

  // Detect and handle button presses
  processButtonPress();

  // Receive and process commands sent from the PC over serial
  processSerialCommand();

  delay(5);
}

/**
 * Detect and handle button presses
 **/
void processButtonPress() {

  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {

    if (buttons[i].update()) { // If button state changed...

      byte buttonNumber = i+1;

      if (buttons[i].fallingEdge()) { // Pressed 
        Joystick.button(buttonNumber, 1);
        Serial.print("buttonPress: ");
        Serial.println(buttonNumber);

      } 
      else { // Released
        Joystick.button(buttonNumber, 0);
        Serial.print("buttonRelease: ");
        Serial.println(buttonNumber);
      }
    }
  }
}

/**
 * Set the RGB LED values
 *
 * r - The intensity of the RED LED, 0 - 255
 * g - The intensity of the GREEN LED, 0 - 255
 * b - The intensity of the BLUE LED, 0 - 255
 * ledNumber - Either 0 or 1, indicates which LED to set
 **/
void setLED(byte r, byte g, byte b, byte ledNumber) {
  byte offset = 3*ledNumber;
  analogWrite(LED_PINS[0+offset], r);
  analogWrite(LED_PINS[1+offset], g);
  analogWrite(LED_PINS[2+offset], b);
}

/**
 * Play a sound through the Piezo buzzer
 * 
 * freq - The frequency of the tone in Hz
 * duration -  The duration of the tone in milliseconds
 **/
void buzz(unsigned long freq, unsigned long duration) { 
  tone(BUZZER_PIN, freq, duration);
}


/**
 * Stop playing the tone
 **/
void noBuzz() {
  noTone(BUZZER_PIN);
}

/**
 * Split the input string into words separated by spaces and return the n-th word
 * 
 * Used to extract serial arguments
 * 
 * http://stackoverflow.com/a/14824108/401554
 **/
String getToken(String data, int index)
{
  char separator = ' ';
  int found = 0;
  int strIndex[] = { 0, -1  };
  int maxIndex = data.length() - 1;

  for(int i = 0; i <= maxIndex && found <= index; i++){
    if(data.charAt(i) == separator || i == maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/**
 * Read serial interface and return a string of sent data
 **/
String getSerialCommand() {

  String serialInput = "";
  char character;

  while(Serial.available()) {
    character = Serial.read();
    serialInput.concat(character);
  }

  return serialInput;
}

/**
 * Receive serial input and proccess it
 **/
void processSerialCommand(){

  String serialInput = getSerialCommand();

  if (serialInput == "") {
    return;
  }

  // The first "word" is always the command
  String command = getToken(serialInput, 0);

  // Numeric params for the command
  unsigned long param1 = stringToInt(getToken(serialInput, 1));
  unsigned long param2 = stringToInt(getToken(serialInput, 2));
  unsigned long param3 = stringToInt(getToken(serialInput, 3));

  if (command == "buzz"){ // Make sound
    buzz(param1, param2);
  } 
  else if (command == "nobuzz") { // Stop the buzzer
    noBuzz();
  } 
  else if (command == "led0") { // Light LED
    setLED(param1 > 255 ? 255 : param1, param2 > 255 ? 255 : param2, param3 > 255 ? 255 : param3, 0);
  } 
  else if (command == "led1") {
    setLED(param1, param2, param3, 1);
  }
  else if (command == "kristo"){ // Easter egg command
    kristo();
  }
  else {
    Serial.println(-1); // Error
  }
}


/**
 * Convert a string into an integer
 *
 * http://christianscode.blogspot.com/2012/05/convert-string-to-integer.html
 **/
int stringToInt(String thisString) {
  int i, value, length;
  length = thisString.length();
  char buffer[(length+1)];

  for(i=0; i<length; i++) {
    buffer[i] = thisString.charAt(i);
  }

  buffer[i]=0;
  value = atoi(buffer);
  return value;
}

/**
 * Play Kristo's theme through the buzz function.
 * 
 * http://garagelab.com/profiles/blogs/how-to-use-tone-function-arduino-playing-the-james-bond-theme
 **/
void kristo(){

  // Note frequencies
  int melody[] = {
    330,349,349,349,349,330,330,330,
    330,392,392,392,392,330,330,330,
    330,349,349,349,349,330,330,330,
    330,392,392,392,392,330,330,330,
    622,587,494,440,494,
    330,392,622,587,392,494,
    494,740,698,494,587,932,
    880,698,880,1245,1175,0
  };

  // Note duration: 1 = whole note, 2 = half note, 4 = quarter note, 8 = eighth note, etc.
  int noteDurations[] = {
    8,16,16,8,4,8,8,8,
    8,16,16,8,4,8,8,8,
    8,16,16,8,4,8,8,8,
    8,16,16,8,4,8,8,8,
    8,2,8,8,1,
    8,4,8,4,8,8,
    8,8,4,8,4,8,
    4,8,4,8,3
  };

  int pace = 1450; // change pace of music("speedy")
  for (int Note = 0; Note <54; Note++) {//counter of Notes (54 limit the array)
    int duration = pace/noteDurations[Note];//Adjust duration with the pace of music
    buzz(melody[Note],duration); //Play note

    // to distinguish the notes, set a minimum time between them.
    delay(duration*1.2);
  }
}
