#include "Arduino.h"
#include "stdbool.h"

typedef struct Panel {
  int x;
  int y;
  int height;
  int width;
} Panel;

const Panel labelPanel = {0, 110, 18, 64};

// testing variables
int counter = 0;
int reversed = false;
//

int getValue() {
  if ((counter < 100) && !reversed) {
    counter++;
  }
  else if ((counter > 0) && reversed) {
    counter--;
  }

  if (counter == 100) {
    reversed = true;
  } else if (counter == 0) {
    reversed = false;
  }

  return counter;
}

void setup() {
  VextON();
  delay(100);
  initDisplay();
}

void loop() {
  drawUI(getValue());
  delay(100);
}
