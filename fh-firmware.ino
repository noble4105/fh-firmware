#include "Arduino.h"
#include "stdbool.h"
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "esp_sleep.h"

//Prototypes so there are no oopsies
double demod(int16_t rssi);
int scaleSignal(int dist, int range);
int findAvg(int* arr);
int getValue();
int fakeVal(void);
void arrStore(int item);
void frequencyHarmonize();
void setupForCoolKids();
void cycleDisplay(uint16_t, int);
void loopRadio();
void listenForButton();
void setupRadio();
void initDisplay();
void shutdownDisplay();
void initScaling();
void pinSetup();
void batteryDisplay();

typedef struct Panel {
  int x;
  int y;
  int height;
  int width;
} Panel;

const Panel labelPanel = {0, 110, 18, 64};

const int avgsize = 6; //Array size for averaging signal strength data

int16_t mainrssi; // Variable for pulling signal strength from pingpong
int16_t tgl = 0; //Variable for only printing and pulling info once per pingpong cycle
int demodded = 0;

int16_t timeout;
int receivedTime = 0;

const int dBMax = -20; // Globals for converting rssi to distance
const int dBMin = -120;
int dBSpan;
const int meterRange = 15000;
float powerForRange;

long battTime;

int avgArray[avgsize]; // array for average of distance values

bool lowpowermode = true;
uint16_t displayState = 0;
long relativeTime, currentTime, pressedTime;
bool waitflag, pushflag, toastState, reset, sleepStarted, isSetup = false; 

int fakePercent =  0; // random value on startup for fake battery percentage

typedef enum
{
    LOWPOWER,
    STATE_RX,
    STATE_TX
}States_t;
States_t state;

void setup() {
  isSetup = false; // Take your setup function somewhere else! >:(
  lowpowermode = true;
  fakePercent = 95 - rand()%9;
  
  setupForCoolKids();
  battTime = millis();
  esp_light_sleep_start();
}

void loop() {  
  if(!isSetup)
  { //temporarily restoring no sleep mode functionality
    isSetup = true;
    setupForCoolKids();
  }

    listenForButton();

    if(!lowpowermode && !waitflag)
    {
      frequencyHarmonize(); //moved implementation to process.ino
    }
  }

void setupForCoolKids()
{
  initDisplay();
  setupRadio();
  initScaling();
  srand(time(0));
  pinSetup();

  delay(20);
  lowpowermode = false;
  digitalWrite(48, HIGH); // 48 is ledpin but it's not being recognized by compiler

  Serial.print("\nReinitializing...\n");

  receivedTime = millis();
  tgl = 0; // if tgl is never reset, the timeout function doesnt work
  searchingForDevices();
}

