#include "Arduino.h"
#include "stdbool.h"
#include <time.h>
#include <stdio.h>
#include <math.h>

//Prototypes so there are no oopsies
double demod(int16_t rssi);
int scaleSignal(int dist, int range);
int findAvg(int* arr);
int getValue();
int fakeVal(void);
void arrStore(int item);

typedef struct Panel {
  int x;
  int y;
  int height;
  int width;
} Panel;

const Panel labelPanel = {0, 110, 18, 64};

const int avgsize = 6; //Array size for averaging signal strength data

int16_t mainrssi; // Variable and pointer for pulling signal strength from pingpong
int16_t* rssiptr = &mainrssi;
int16_t tgl = 0; //Variable and pointer for only printing and pulling info once per pingpong cycle
int16_t* tglptr = &tgl;

int16_t timeout;
int16_t* timeoutptr = &timeout;

const int dBMax = -20; // Globals for converting rssi to distance
const int dBMin = -120;
int dBSpan;
const int meterRange = 15000;
float powerForRange;

int avgArray[avgsize]; // array for average of distance values

void setup() {
  VextON();
  delay(100);
  initDisplay();
  setupRadio();
  initScaling();
  srand(time(0));
}

void loop() {
  int currentTime = millis();
  int receivedTime; 
  timeout = 3000 + rand()%5001;

  loopRadio();

  if(tgl == 1)
  {
    //int demodded = round(demod(mainrssi));REPLACED BY NICKS FUNCTION // Interpret signal strength from pingpong

    arrStore(mainrssi); // Shift newest demodded value into the averaging array

    int finalAverage = findAvg(avgArray); // Compute latest average signal strength

    int demodded = round(dBToMeters(finalAverage));

    int scaled = scaleSignal(demodded, 115); // Scale average to usable value


    for(int i = 0; i < avgsize; i++)
    {
      Serial.printf("\nEntry %i is %i", i, avgArray[i]);
    }

    //Print numbers for testing purposes
    Serial.printf("\nrssi returned is %i, averaged rssi %i, meter conversion %i, scaled %i\n", mainrssi, finalAverage, demodded, scaled);

    drawCircles(scaled); //Display on screen!!!

    receivedTime = millis(); 

    tgl = 0; // Reset toggle so this only happens when new data is pulled
  }

  if((currentTime - receivedTime) > 10000 && tgl != 2)
  {
    Serial.printf("\nTime diff is %i", (currentTime-receivedTime));
    noDevices();
    tgl = 2;
  }
}
