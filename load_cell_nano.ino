#include <HX711_ADC.h>
#include <EEPROM.h>

#define HX711_DOUT 4
#define HX711_SCK 5


HX711_ADC loadcell(HX711_DOUT, HX711_SCK);
int weight;

void setup()
{
  Serial.begin(9600);
  loadcell.begin();


  float calib_value = 0.0;
  EEPROM.get(0, calib_value);
  unsigned long stabilizingtime = 2000;
  loadcell.start(2000, true);

  if (loadcell.getTareTimeoutFlag()) {
    //    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
  }
  else {
    loadcell.setCalFactor(calib_value); // set calibration factor (float)
    //    Serial.println("Startup is complete");
  }

  while (!loadcell.update());
  if (loadcell.getSPS() < 7) {
    //    Serial.println("!!Sampling rate is lower than specification, check MCU>HX711 wiring and pin designations");
  }
  else if (loadcell.getSPS() > 100) {
    //    Serial.println("!!Sampling rate is higher than specification, check MCU>HX711 wiring and pin designations");
  }

  loadcell.tareNoDelay();

  if (loadcell.getTareStatus() == true) {
    //    Serial.println("Tare complete");
  }

}

void loop()
{

  static boolean newDataReady = 0;
  static unsigned long t = 0;

  if (loadcell.update()) newDataReady = true;

  if (newDataReady) {
    if (millis() - t >= 300) {
      weight = int( loadcell.getData() / 1000);
      newDataReady = 0;
      Serial.println(weight);
      t = millis();
    }
  }

}
