/*New ino file for mapping button inputs and outputting for LEDs*/
#include "Arduino.h"

const int buttonpin = 46;
const int ledpin = 48;

void pinSetup()
{
  pinMode(buttonpin, INPUT);
  pinMode(buttonpin, INPUT_PULLUP); //LOW is pressed, HIGH is not pressed

  pinMode(ledpin, OUTPUT);
}

void shortpress()
{
  if(!lowpowermode) // short press should only work if not in low power mode
  {
    displayState++; // We may need to add something for button bounce
  }
}

//Used explicitly to switch between low power mode and operating mode
void longpress()
{ 
 if(!lowpowermode)
 {
  lowpowermode = true; //Low power mode
  digitalWrite(ledpin, LOW);
  displayState = 0; // Resets display style every time you switch to low power
 }
 else
 {
  lowpowermode = false; // Operating mode
  digitalWrite(ledpin, HIGH);
 }
}

//Logic for detecting if button press is long or not
void listenForButton()
{
  long relativeTime, currentTime, pressedTime;

  if(digitalRead(buttonpin) == LOW) //Only operates when button is detected
  {
    relativeTime = millis();

    while(digitalRead(buttonpin) == LOW) //While button is pressed, keep count of how long it is
    {
      currentTime = millis();
    }

    pressedTime = currentTime - relativeTime; //Calculates how long the button was pressed for

    if(pressedTime < 5000) // Long press is 5 seconds, so if less than 5 seconds, short press
    {
      shortpress();
    }
    else 
    {
      longpress();
    }
  }
}

