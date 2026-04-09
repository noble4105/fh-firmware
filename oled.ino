#include "HT_SSD1306Wire.h"

static SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst

const int BAR_HEIGHT = 10;
const int BAR_WIDTH = 42;
const int BAR_BUFFER = 2;
const int STEP = BAR_BUFFER + BAR_HEIGHT;

const int displayStyleCount = 2; // Used as modulus factor in cycling displays

// X = 0, Y = 0 IS AT TOP LEFT WHEN PORT IS FACING DOWN
// X MAX IS 64 I THINK
// Y MAX IS 128 I THINK

int scaleSignal(int dist, int range);


void VextON(void) {
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void) {
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
}

void initDisplay() {
  VextON();
  delay(100);
  display.init();
  display.clear();
  display.display();
  display.setContrast(255);
  display.clear();
  display.display();
  display.screenRotate(ANGLE_270_DEGREE);
}

void drawBars(int val) {
  display.clear();

  int vTrack = 8 * STEP;

  for (int i = 9; i>0; i--) {
    if (val > ((10-i)*10)) {
      display.fillRect((64-BAR_WIDTH), vTrack, BAR_WIDTH, BAR_HEIGHT);
    }
    else {
      display.drawRect((64-BAR_WIDTH), vTrack, BAR_WIDTH, BAR_HEIGHT);
    }
    vTrack -= STEP;
  }
}

void clearPanel(Panel panel) {
  for (int i = panel.y; i < (panel.y + panel.height); i++) {
    for (int j = panel.x; j < panel.width; j++) {
      display.clearPixel(j, i);
    }
  }
}

void drawValString(int val) {
  bool isKilo = false;
  String message = "";
  message = String(val) + "m";

  if (val > 1000) {
    double doubleValue = val / 1000.0;
    isKilo = true;
    message = String(doubleValue, 2) + "km";
  }

  clearPanel(labelPanel);
  display.setFont(ArialMT_Plain_10);
  display.drawString(38, 115-10/2, message);
}

void drawUI(int val) {
  int adjustedVal = 0;

  if (val > 100) {
    adjustedVal = 0;
  } else if (val > 90) {
    adjustedVal = 10;
  } else if (val > 80) {
    adjustedVal = 20;
  } else if (val > 70) {
    adjustedVal = 30;
  } else if (val > 60) {
    adjustedVal = 40;
  } else if (val > 50) {
    adjustedVal = 50;
  } else if (val > 40) {
    adjustedVal = 60;
  } else if (val > 30) {
    adjustedVal = 70;
  } else if (val > 20) {
    adjustedVal = 80;
  } else if (val > 10) {
    adjustedVal = 90;
  } else {
    adjustedVal = 100;
  }

  drawBars(adjustedVal);
  drawValString(demodded); //Changed to demodded. 
  //It will show proper meter length but the bars filling is according to scaledMax

  displayPercent();

  display.display();
}

void drawCircles(int val) // needs work on string positioning
{
  // Input range is around 0-117
  display.clear();
  display.drawString(5, 110, "You");
  
  display.drawString(38, (111-val), "Them");
  display.drawCircle(30, (117-val), 5);

  display.drawLine(30, 5, 30, (109-val)); // Draw line from top to circle radius
  display.drawLine(30, 123, 30, (124-val)); // Draw line from bottom to circle radius

  displayPercent();

  display.display();
}

void noDevices(void) // display mode for no connection
{
  toastState = true; // COMMENT OUT THIS LINE IF YOU ARE SOLO TESTING
  display.clear();
  display.drawString(35, 54, "No");
  display.drawString(25, 64, "devices");
  display.drawString(30, 74, "found");
  
  displayPercent();

  display.display();
}

void searchingForDevices(void) // display mode for startup
{
  toastState = true; // COMMENT OUT THIS LINE IF YOU ARE SOLO TESTING
  display.clear();
  display.drawString(20, 54, "Searching");
  display.drawString(35, 64, "for");
  display.drawString(25, 74, "devices");
  
  displayPercent();

  display.display();
}

//the display state value gets changed in the in-out.ino
//whenever a short button press is detected. The 
// displayStyleCount is defined at the top,
// so we can change it when we overhaul display
void cycleDisplay(uint16_t displayState, int scaledValue)
{
  int state = displayState % displayStyleCount;

  switch(state)
  {
    case 0:
    if(reset)
    {
      display.clear();
      reset = false;
    }
    drawCircles(scaleSignal(scaledValue, 115));
    break;

    case 1:
    if(reset)
    {
      display.clear();
      reset = false;
    }
    drawUI(scaleSignal(scaledValue, 100));
    break;
  }
}

void shutdownDisplay()
{
  display.clear();
  display.display();
  VextOFF();
  delay(100);
}

void batteryDisplay()
{
  static int lastVolt = 0;
  int voltVal = batteryRead();

  if(voltVal != lastVolt)
  {
    display.clear();

    char voltString[20];
    sprintf(voltString, "%d", voltVal);

    display.drawString(16, 16, voltString);
    display.display();

    lastVolt = voltVal;

    delay(500);
  }
}

void displayPercent()
{
  char battval[5];

  sprintf(battval, "%i", samplePercent());

  display.drawString(1, 54, "Bat.");
  display.drawString(2, 64, battval);
  display.drawString(3, 74, "%");
}

int samplePercent()
{
  long current = millis();

  long timeToBatt = current - battTime;

  if(timeToBatt < 60000)
  {
    return fakePercent;
  }
  else if(timeToBatt < 237000)
  {
    return fakePercent-1;
  }
  else if(timeToBatt < 400000)
  {
    return fakePercent-2;
  }
  else if(timeToBatt < 650000)
  {
    return fakePercent-3;
  }
  else if(timeToBatt < 910000)
  {
    return fakePercent-4;
  }
  else if(timeToBatt < 1400000)
  {
    return fakePercent-5;
  }
  else if(timeToBatt < 1900000)
  {
    return fakePercent-6;
  }
  else{
    return 37;
  }
}

