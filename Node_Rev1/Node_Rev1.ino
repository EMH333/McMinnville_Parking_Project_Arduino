#include "UltrasonicDetection.h"
#include "radio.h"
//#include "runningAverage.h"


void setup()
{
  initRadios();
  initUltrasoic();
  //initAverage();
  Serial.begin(9600);
}


void loop()
{
   sendCarData(getCarInformation());
   sendCarData(getCarInformation());
   delay(1000);
}
