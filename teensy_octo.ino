/**
 * Teensy Octo
 * 
 * @author <david@sqroot.eu>
 * 
 * Useful links:
 * 
 * http://www.pjrc.com/teensy/td_libs_Bounce.html - About button bouncing
 * http://www.pjrc.com/teensy/pinout.html - Teensy pinout diagram
**/

#include <Bounce.h>

#define DEBOUNCE 10  // Number of ms to debounce
#define NUMBER_OF_BUTTONS 8

// Define digital pins for the buttons
const byte BUTTON_PINS[] = {0, 1, 2, 3, 13, 16, 17, 18};

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

// Define PWM pins for the RGB LED
const byte RGB_RED = 4;
const byte RGB_GREEN = 5;
const byte RGB_BLUE = 9;

// The Piezo buzzer sits in this PWM pin
const byte BUZZER_PIN = 12;

/**
 * Setup function - run once on power-on
**/
void setup() {
  Serial.begin(9600);
  
  // Initialize pushbutton pins as inputs with the internal pullup resistor enabled
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
      pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  // Set RGB LED pins as output
  pinMode(RGB_RED, OUTPUT);  
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  
  // The LED is OFF by default
  setLED(255, 255, 255);
  
  // Onboard LED to ON for debugging
  pinMode(11, OUTPUT);  
  digitalWrite(11, HIGH);

  Serial.println("Teensy Octo v1.0 by A. Roots 2013. Who are you?"); // I wonder whether he gets it without reading the source...
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
  
  delay(10);
}

/**
 * Detect and handle button presses
**/
void processButtonPress() {
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    if (buttons[i].update() && buttons[i].read()) {
      
      Serial.println(i);  
      buzz(i*30 + 30, 500);
    
    }
  }
}

/**
 * Set the RGB LED values
 *
 * r - The intensity of the RED LED, 0 - 255
 * g - The intensity of the GREEN LED, 0 - 255
 * b - The intensity of the BLUE LED, 0 - 255
**/
void setLED(byte r, byte g, byte b){
  analogWrite(RGB_RED, r);
  analogWrite(RGB_GREEN, g);
  analogWrite(RGB_BLUE, b);
}

/**
 * Play a sound through the Piezo buzzer
 * 
 * freq - The frequency of the tone in Hz
 * duration -  The duration of the tone in milliseconds
**/
void buzz(unsigned int freq, unsigned long duration){ 
  tone(BUZZER_PIN, freq, duration);
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
    buzz(param1, param2);
  } else if (command == "led") { // Light LED
    setLED(param1, param2, param3);
  } else if (command == "help") { // Get help
      Serial.println("http://github.com/anroots/teensy_octo");
  } else if (command == "kristo"){ // Easteregg
    kristo();
  } else {
      Serial.print("Unknown command: "); 
      Serial.println(command);
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
