#include "Joystick.h"

// Create Joystick
Joystick_ Joystick;
int xPin = A1;
int yPin = A0;
int buttonPin = 2;

int xPosition = 0;
int yPosition = 0;
int buttonState = 0;
int factor=2;
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  //activate pull-up resistor on the push-button pin
  pinMode(buttonPin, INPUT_PULLUP); 
  Joystick.setXAxisRange(-255, 255);
  Joystick.setYAxisRange(-255, 255);
  Joystick.begin();

  
}

void loop() {
  xPosition = (analogRead(xPin)/factor)-255;
  yPosition = (analogRead(yPin)/factor)-255;
  buttonState = digitalRead(buttonPin);
  Joystick.setXAxis(xPosition);
  Joystick.setYAxis(yPosition);
  Serial.print("X: ");
  Serial.print(xPosition);
  Serial.print(" | Y: ");
  Serial.print(yPosition);
  Serial.print(" | Button: ");
  Serial.println(buttonState);

  delay(10); // add some delay between reads
}
