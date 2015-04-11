/****************************
 * Read MPL115A2
 *  An example sketch that prints the
 *  local pressure to the PC's serial port
 *
 *  Tested with the MPL115A2-Breakout
 *  Pressure Sensor from Misenso Electronics
 *****************************/

#include <Wire.h>
#include <MPL115A2.h>

void setup()
{
  MPL115A2.begin();
  MPL115A2.shutdown();
  Serial.begin(9600);
}

void loop()
{
  MPL115A2.ReadSensor();
  MPL115A2.shutdown();
  
  Serial.print("Pressure(kPa): ");
  Serial.print(MPL115A2.GetPressure());
  Serial.print("     Temperature(C): ");
  Serial.println(MPL115A2.GetTemperature());
  
  delay(1000);
}
