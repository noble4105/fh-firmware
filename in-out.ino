/*New ino file for mapping button inputs and outputting for LEDs*/
#include "Arduino.h"

const int buttonpin = 46;
const int ledpin = 48;
int res = 0;

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
    Serial.print("\ndisplay cycled\n");
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
  shutdownDisplay();
 }
 else
 {
  lowpowermode = false; // Operating mode
  digitalWrite(ledpin, HIGH);
  setupRadio();
  initDisplay();
 }
}

//Logic for detecting if button press is long or not
void listenForButton()
{
  long relativeTime, currentTime, pressedTime = 0;

  if(digitalRead(buttonpin) == HIGH) // Resets only when the button is released
  {
    res = 1;
  } 

  if(digitalRead(buttonpin) == LOW) //Only operates when button is detected
  {
    relativeTime = millis();
    

    while(digitalRead(buttonpin) == LOW && pressedTime <= 2000)  //While button is pressed, keep count of how long it is
    {
      currentTime = millis();
      pressedTime = currentTime - relativeTime; //Calculates how long the button was pressed for
    }

    

    if(pressedTime < 2000 && pressedTime > 50 && res == 1) // Long press is 5 seconds, so if less than 5 seconds, short press
    {                      // >100 to do nothing for button bounce
      shortpress();
      res = 0;
    }
    else if (pressedTime >= 2000 && res == 1)
    {
      longpress();
      res = 0;
    }
  }
}

