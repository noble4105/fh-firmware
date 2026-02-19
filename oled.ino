#include "HT_SSD1306Wire.h"

static SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst

const int BAR_HEIGHT = 10;
const int BAR_WIDTH = 64;
const int BAR_BUFFER = 2;
const int STEP = BAR_BUFFER + BAR_HEIGHT;

void VextON(void) {
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void) {
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
}

void initDisplay() {
  display.init();
  display.clear();
  display.display();
  display.setContrast(255);
  display.clear();
  display.display();
  display.screenRotate(ANGLE_90_DEGREE);
}

void drawBars(int val) {
  if (val % 10 == 0) { // reset when rect state change instead otherwise jank
    display.clear();
  }

  int vTrack = 8 * STEP;

  for (int i = 9; i>0; i--) {
    if (val > ((10-i)*10)) {
      display.fillRect(0, vTrack, BAR_WIDTH, BAR_HEIGHT);
    }
    else {
      display.drawRect(0, vTrack, BAR_WIDTH, BAR_HEIGHT);
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
  clearPanel(labelPanel);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(32, 115-10/2, String(val) + "%");
}

void drawUI(int val) {
  drawBars(val);
  drawValString(val);
  display.display();
}

void drawCircles(int16_t val)
{
  // Input range is around 0-117
  display.clear();
  display.drawCircle( 7, (7+val), 5);
  display.display();
  delay(800);
}


