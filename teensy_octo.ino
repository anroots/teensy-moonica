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
const int RGB_RED = 4;
const int RGB_GREEN = 20;
const int RGB_BLUE = 21;

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
  analogWrite(RGB_GREEN, 255);
  analogWrite(RGB_BLUE, 255);
  
  pinMode(11, OUTPUT);  
}

void checkButtons() {
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
      if (buttons[i].update() && buttons[i].read()) {
        Serial.println(i);
    }
}
}

void loop()                     
{
    
  checkButtons(); 

  // Blink LED for debugging, just to see that it works
  digitalWrite(11, HIGH);
  delay(100);
  digitalWrite(11, LOW); 
  delay(300);  
}



void buttonPress(int buttonNumber) {
  Serial.println(buttonNumber);
  switch (buttonNumber){
   case 0:
     analogWrite(RGB_RED, 200);
     break;
   case 1:
     analogWrite(RGB_GREEN, 200);
     break;
   case 2:
     analogWrite(RGB_BLUE, 200);
     break;
  }
}

