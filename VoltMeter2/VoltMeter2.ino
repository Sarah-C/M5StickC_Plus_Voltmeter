
#include "M5StickCPlus.h"
#include "M5_ADS1115.h"

ADS1115 voltmeter;

/*

 Voltage = Raw read
 10.009836 = 20395
 10.009345 = 20394
 Therefore, each single bit rise = 0.000491 volts
 So meaningful digits are xx.xxx

65535 (2 bytes) * 0.000491 = 32.177685 volts.
Highest accuracy reading of 3 decimal places, and up to 30 volts = PAG_256
(The circuit's not built to read higher volts? Not worth setting gain differently)

 */

void setup() {
  M5.begin();
  Wire.begin();
  voltmeter.setMode(SINGLESHOT);
  voltmeter.setRate(RATE_8); // Lowest most stable read time.
  voltmeter.setGain(PAG_256); // Most sensitive detection.
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(WHITE, BLACK);
}

void loop(void) {
  double volts = 0;
  volts = voltmeter.getValue() / 1000.0; // Convert mV into volts.
  if(volts < 0.000001) volts = 0.0; // Random noise causes negative values.
  M5.Lcd.setCursor(30, 40);
  M5.Lcd.printf("%.3lf v   ", volts); 
}
