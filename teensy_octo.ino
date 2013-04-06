/**
http://www.pjrc.com/teensy/td_libs_Bounce.html
http://www.pjrc.com/teensy/pinout.html
**/

#include <Bounce.h>


#define DEBOUNCE 10  // Number of ms to debounce

#define NUMBER_OF_BUTTONS 8

// Define pins for the buttons
const byte BUTTON_PINS[] = {0, 1, 2, 3, 13, 16, 17, 18};

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

// Define pins for the RGB LED
const byte RGB_RED = 4;
const byte RGB_GREEN = 5;
const byte RGB_BLUE = 9;

const byte BUZZER_PIN = 12;

void setup()   {
  Serial.begin(38400);
  
  // Initialize pushbutton pins
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
      pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  pinMode(RGB_RED, OUTPUT);  
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  
  analogWrite(RGB_RED, 250);
  analogWrite(RGB_GREEN, 250);
  analogWrite(RGB_BLUE, 250);
  
  // Onboard LED to ON
  pinMode(11, OUTPUT);  
  digitalWrite(11, HIGH);
}

void buzz(unsigned int freq, unsigned long duration){
  tone(BUZZER_PIN, freq, duration);
}

void loop()                     
{
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    if (buttons[i].update() && buttons[i].read()) {
      buttonPress(i);
    }
  }
  int bytecount=0;
  byte incomingByte;
   while (Serial.available() && bytecount < 10) {
    incomingByte = Serial.read();
Serial.println(incomingByte);
if (incomingByte == 49){
  buzz(100,900);
}
if (incomingByte == 50){
  buzz(300,300);
}

    bytecount++;
  }
}

void buttonPress(int buttonNumber) {
  Serial.println(buttonNumber);
  
  static int r;
  static int g;
  static int b;
  
  
  
  buzz(buttonNumber*30 + 30, 500);
}

