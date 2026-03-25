/*New ino file for mapping button inputs and outputting for LEDs*/
#include "Arduino.h"
#include <Wire.h>
#include "heltec.h"

const int buttonpin = 46;
const int ledpin = 48;
bool res = true;

void buttonPush() 
{
  if(digitalRead(buttonpin) == LOW)
  {
    pushflag = true;
    waitflag = true;
    res = true;
    relativeTime = millis();
  }
  else
  {
    pushflag = false;
  }
}

void pinSetup()
{
  pinMode(buttonpin, INPUT);
  pinMode(buttonpin, INPUT_PULLUP); //LOW is pressed, HIGH is not pressed

  pinMode(ledpin, OUTPUT);

  attachInterrupt(buttonpin, buttonPush, CHANGE);


/* This is the start of battery reading from example Battery_power.ino
  analogSetClockDiv(1);
  analogSetAttenuation(ADC_11db);
  analogSetPinAttenuation(36,ADC_11db);*/
}

void shortpress()
{
  if(!lowpowermode) // short press should only work if not in low power mode
  {
    reset = true;
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
  shutdownDisplay();
 }
 else
 {
  lowpowermode = false; // Operating mode
  digitalWrite(ledpin, HIGH);
  setupRadio();
  initDisplay();
  tgl = 0;
 }
}


//Logic for detecting if button press is long or not
void listenForButton()
{
  currentTime, pressedTime = 0;

  if(waitflag) //Only operates when button is detected
  {

    currentTime = millis();//This exists if the button press was during a radio process. 
                           //It can still see a short press since relative time is set during button press.

    while(pushflag && pressedTime <= 2000)  //While button is pressed, keep count of how long it is
    {
      currentTime = millis();
      pressedTime = currentTime - relativeTime; //Calculates how long the button was pressed for
    }

    if(pressedTime < 2000 && res && !lowpowermode) // Long press is 5 seconds, so if less than 5 seconds, short press
    {              
      if(!toastState) //Dont cycle displays if it's showing no devices found
      {
        shortpress();
        cycleDisplay(displayState, scaled); //Cycles dislay, scaled scaled shouldn't be changed so data doesnt change only display style
      }        

      waitflag = false; //continue FH
      res = false; // stops it from detecting multiple button presses from just one
    }

    else if (pressedTime >= 2000 && res)
    {
      longpress();
      waitflag = false; //continue FH
      res = false; // stops from detecting multiple button presses from just one
    }
  }
}

