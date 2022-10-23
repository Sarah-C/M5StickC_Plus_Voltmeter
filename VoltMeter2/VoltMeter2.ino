
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

// Polynomials from here:
// https://github.com/G6EJD/LiPo_Battery_Capacity_Estimator/blob/master/ReadBatteryCapacity_LIPO.ino
float getBatteryPercent(){
  float voltage = M5.Axp.GetBatVoltage();
  float batteryPercent = 2808.3808 * pow(voltage, 4) - 43560.9157 * pow(voltage, 3) + 252848.5888 * pow(voltage, 2) - 650767.4615 * voltage + 626532.5703;
  if(batteryPercent < 0) batteryPercent = 0;
  if(batteryPercent > 100) batteryPercent = 100;
  return batteryPercent;
}

float getBatteryLevel(void){
  float emptyBatVoltage = 3.1f;
  float fullBatVoltage = 4.15;
  float vBat = M5.Axp.GetBatVoltage();
  float batteryPercent = 100.0f * ((vBat - emptyBatVoltage) / (fullBatVoltage - emptyBatVoltage));
  if(batteryPercent > 100) batteryPercent = 100;
  if(batteryPercent < 0) batteryPercent = 0;
  return batteryPercent;
}

void setup(){
  M5.begin();
  Wire.begin();
  voltmeter.setMode(SINGLESHOT);
  voltmeter.setRate(RATE_8); // Lowest most stable read time.
  voltmeter.setGain(PAG_256); // Most sensitive detection.
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(WHITE, BLACK);
}

void displayBattery(){
  M5.Lcd.setCursor(30, 10);  
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf("Battery : %.0f %% ", getBatteryPercent());
  M5.Lcd.setCursor(155, 10);  
  M5.Lcd.printf("%.2f v   ", M5.Axp.GetBatVoltage());
}

void displayVoltage(){
  double volts = 0;
  volts = voltmeter.getValue() / 1000.0;
  if(volts < 0.000001) volts = 0.0;
  M5.Lcd.setCursor(30, 40);  
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("%.3lf v   ", volts); 
}

void loop(void){
  displayVoltage();
  displayBattery();
}
