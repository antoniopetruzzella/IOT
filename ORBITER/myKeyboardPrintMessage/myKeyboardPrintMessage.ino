#include "Keyboard.h"

const int buttonPin = 4;          // input pin for pushbutton
int previousButtonState = HIGH;   // for checking the state of a pushButton
int counter = 0; // button push counter
char ctrlKey=128;
char shfKey=129;
char altKey=130;


void setup() {
  // make the pushButton pin an input:
   Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  // initialize control over the keyboard:
  Keyboard.begin();
}

void loop() {
  // read the pushbutton:
  int buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  // if the button state has changed,
  if ((buttonState != previousButtonState)
      // and it's currently pressed:
      && (buttonState == HIGH)) {
        Serial.println("HIGH");
    // increment the button counter
    counter++;
    // type out a message
    
    /*Keyboard.press(ctrlKey);PROCEDURA DI UNDOCKING
   
    Keyboard.press(100);
    delay(100);
    */
    
    //Keyboard.press(altKey);
    Keyboard.press(194);
     delay(100);
    Keyboard.releaseAll();
    //Keyboard.print("p903s42");
    
    /* PROCEDURA DI DECOLLO AUTOMATICO
    Keyboard.write(112);
    delay(100);
    Keyboard.write(57);
   delay(100);
    Keyboard.write(48);
    delay(100);
    Keyboard.write(51);
    delay(100);
    Keyboard.write(115);
    delay(100);
    Keyboard.write(52);
    delay(100);
    Keyboard.write(50);
    delay(100);    
    Keyboard.write(176);
    delay(100);
    Keyboard.write(101);*/
  }
  // save the current button state for comparison next time:
  previousButtonState = buttonState;
}
