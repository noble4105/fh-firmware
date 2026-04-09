/*  
  ADC read voltage via GPIO1 simple test.
  Base on Wi_Fi LoRa 32 V3.2
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
*/

void batterySetup() { // used in pinsetup function from in-out.ino

  // Set the resolution of the analog-to-digital converter (ADC) to 12 bits (0-4095):
  analogReadResolution(12);

  // Set pin 37 as an output pin (used for ADC control):
  pinMode(37, OUTPUT);

  // Set pin 37 to HIGH (enable ADC control):
  digitalWrite(37, HIGH);
}

	

int batteryRead() {
  // Read the raw analog value from pin 1 (range: 0-4095 for 12-bit resolution):
  int analogValue = analogRead(1);

  float analogVolts = (analogValue / 4095) * 4.2;

  Serial.printf("analog volts: %.2f\n", analogVolts);

  return round(analogVolts);
  }

