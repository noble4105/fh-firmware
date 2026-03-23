/*New ino file for mapping button inputs and outputting for LEDs*/
#include "Arduino.h"


const int buttonpin = 46;
const int ledpin = 48;
bool res = true;

void IRAM_ATTR buttonPush() 
{
  if(digitalRead(buttonpin) == LOW)
  {
    pushflag = true;
    waitflag = true;
  }
  else
  {
    res = true;
    pushflag = false;
  }
}

/*
void buttonISR() // This doesnt work how i want it to rn
{
  if(res)
  {
    if(pushflag)
    {
      pushTime = millis();
      pushflag = false;
      Serial.printf("press detected, press time is %i, timeout is %i\n", pushTime, buttonTimeout);
    }
    
    if(releaseflag)
    {      
      releaseTime = millis();
      releaseflag = false;
      Serial.printf("release detected, releasetime is %i, timeout is %i \n", releaseTime, buttonTimeout);
      diffTime = releaseTime - pushTime;

      if(diffTime < 2000)
      {
        shortpress();
        res = false;
      }
      else
      {
        longpress();
        res = false;
      }
    }

    if(buttonTimeout - pushTime > 2000 && waitflag)
    {
      longpress();
      res = false;
    }
  }
}

void ListenForButton()
{
  buttonTimeout = millis();

  if(buttonTimeout - pushTime > 2000 && waitflag && res)
  {
    longpress();
    res = false;
  }
  
  if(releaseTime - pushTime <= 2000 && !waitflag && res) 
  {
    shortpress();
  }
}
*/

void pinSetup()
{
  pinMode(buttonpin, INPUT);
  pinMode(buttonpin, INPUT_PULLUP); //LOW is pressed, HIGH is not pressed

  pinMode(ledpin, OUTPUT);

  attachInterrupt(buttonpin, buttonPush, CHANGE);
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
  tgl = 0;
 }
}


//Logic for detecting if button press is long or not
void listenForButton()
{
  relativeTime, currentTime, pressedTime = 0;

  if(pushflag) //Only operates when button is detected
  {
    relativeTime = millis();
    

    while(pushflag && pressedTime <= 2000)  //While button is pressed, keep count of how long it is
    {
      currentTime = millis();
      pressedTime = currentTime - relativeTime; //Calculates how long the button was pressed for
    }

    if(pressedTime < 2000 && res) // Long press is 5 seconds, so if less than 5 seconds, short press
    {                      
      shortpress();
      cycleDisplay(displayState, scaled); //Cycles dislay, scaled scaled shouldn't be changed so data doesnt change only display style
      waitflag = false; //continue FH
      res = false; // stops it from detecting multiple button presses from just one
      state = STATE_RX;

      Serial.print("I just shortpressed");
    }

    else if (pressedTime >= 2000 && res)
    {
      longpress();
      waitflag = false; //continue FH
      res = false; // stops from detecting multiple button presses from just one
      Serial.print("I just longpressed");
    }
  }
}

