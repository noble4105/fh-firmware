#include "Arduino.h"
#include "stdbool.h"
#include <time.h>
#include <stdio.h>
#include <math.h>

//Prototypes so there are no oopsies
float demod(int16_t rssi);
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

int avgArray[avgsize];

void setup() {
  VextON();
  delay(100);
  srand(time(0));
  initDisplay();
  setupRadio();
}

void loop() {

  loopRadio();

  if(tgl == 1)
  {
    int demodded = floor(demod(mainrssi)); // Interpret signal strength from pingpong

    arrStore(demodded); // Shift newest demodded value into the averaging array

    int finalAverage = findAvg(avgArray); // Compute latest average signal strength

    int scaled = scaleSignal(finalAverage, 117); // Scale average to usable value

    for(int i = 0; i < avgsize; i++)
    {
      Serial.printf("\nEntry %i is %i", i, avgArray[i]);
    }

    //Print numbers for testing purposes
    Serial.printf("\nrssi returned is %i, demodded %i, averaged %i, scaled %i\n", mainrssi, demodded, finalAverage, scaled);

    drawCircles(scaled); //Display on screen!!!

    tgl = 0; // Reset toggle so this only happens when new data is pulled
  }
}
