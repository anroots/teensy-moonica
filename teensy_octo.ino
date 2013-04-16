/**
 * Teensy Octo v1.0
 *
 * Custom-built USB peripheral with eight buttons, two PWM RGB LEDs and a Piezo buzzer.
 * Based on the Teensy 2 microcontroller - http://www.pjrc.com/teensy
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
**/

#include <Bounce.h>

#define DEBOUNCE 10  // Number of ms to debounce
#define NUMBER_OF_BUTTONS 8
#define NUMBER_OF_LEDS 6 // 2 RGB LEDs

// Define digital pins for the buttons
const byte BUTTON_PINS[] = {0, 1, 2, 3, 7, 8, 20, 21};

// Define PWM pins for the RGB LED
// LED0-R, LED0-G, LED0-B; LED1-R, LED1-G, LED1-B
const byte LED_PINS[] = {9, 5, 10, 14, 12, 15};

// The Piezo buzzer sits in this PWM pin
const byte BUZZER_PIN = 4;

// Initialize button objects
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
  Serial.begin(9600);
  
  // Initialize pushbutton pins as inputs with the internal pullup resistor enabled
  for (byte i = 0; i < NUMBER_OF_BUTTONS; i++) {
      pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  // Initialize LED pins
  for (byte i = 0; i < NUMBER_OF_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    analogWrite(LED_PINS[i], 255);
  }

  // Print welcome message over serial
  Serial.print("Teensy Octo by A. Roots 2013. Booted in ");
  Serial.print(millis());
  Serial.println("ms - which is amazingly fast.");

  printHelp();
  Serial.println("");
  Serial.println("Waiting for button presses or serial commands.");
  Serial.println("");
}

/**
 * Main loop - the program keeps repeating this forever, as long as there is power
**/
void loop()                     
{
  
  // Detect and handle button presses
  processButtonPress();
  
  // Receive and proccess commands sent from the PC over serial
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
          buzz(80*(i+1), 100);
          Joystick.button(buttonNumber, 1);
          Serial.print("buttonPress: ");
          Serial.println(buttonNumber);

      } else { // Released
          Joystick.button(buttonNumber, 0);
          Serial.print("buttonRelease: ");
          Serial.println(buttonNumber);
          
          // Trigger easter egg?
          if (triggerEasterEgg(i)) {
            kristo();
          }
      }
    }
  }
}

/**
 * A little state machine for detecting button combinations and triggering the easter egg
**/
bool triggerEasterEgg(int currentButton){

  static unsigned long lastButtonPressedOn = 0; 

  static byte easterEggStateMachine[3] = {}; // Last three button presses
  static byte currentIndex = 0; // Current index in the easterEggStateMachine
  byte secretCombination[3] = {0, 0, 7}; // "Correct" combination
  
  if (currentIndex > 0 && millis() - lastButtonPressedOn > 3000){
    currentIndex = 0; // Reset sequence, too slow!
    lastButtonPressedOn = millis();
  }

  // Save current button
  easterEggStateMachine[currentIndex] = currentButton;

  if (currentIndex == 2) { // All three buttons pressed
    currentIndex = 0;

    // Check that the last three buttons were indeed entered in the correct sequence
    for (int i = 0; i<2; i++) {
      if (secretCombination[i] != easterEggStateMachine[i]) {
        return false;
      }
    }
    return true; // All three numbers matched
  }
  
  currentIndex++;
  lastButtonPressedOn = millis();
  return false;
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
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
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
    startWork();
    buzz(param1, param2);
    endWork();
  } else if (command == "nobuzz") {
    startWork();
    noBuzz();
    endWork();
  } else if (command == "led0") { // Light LED
    startWork();
    setLED(param1 > 255 ? 255 : param1, param2 > 255 ? 255 : param2, param3 > 255 ? 255 : param3, 0);
    endWork();
  } else if (command == "led1") {
    startWork();
    setLED(param1, param2, param3, 1);
    endWork();
  } else if (command == "help") { // Get help
      printHelp();
  } else if (command == "kristo"){ // Easter egg
    startWork();
    kristo();
    endWork();
  } else {
      Serial.print("Unknown command: "); 
      Serial.println(command);
  }
}


/**
 * Print short help message
**/
void printHelp(){
  Serial.println("");
  Serial.println("=== Available serial commands: ===");
  Serial.println("");
  Serial.println("buzz frequency duration");
  Serial.println("nobuzz");
  Serial.println("led0 r g b");
  Serial.println("led1 r g b");
  Serial.println("");
  Serial.println("More info: http://github.com/anroots/teensy-octo");  
}

/**
 * Indicate that the device is currently busy
**/
void startWork(){
  Serial.println("Working...");
}

/**
 * Indicate that the device is ready to receive commands
**/
void endWork(){
  Serial.println("Ready.");
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
