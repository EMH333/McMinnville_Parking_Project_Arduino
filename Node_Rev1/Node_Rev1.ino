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
  if(hasCarPassed(readDistanceOne(),readDistanceTwo(),millis())){
    Serial.println("CAR PASSED");
    sendCarData(getCarInformation());
  }
}
