#include "UltrasonicDetection.h"
#include "radio.h"
#include "runningAverage.h"


void setup()
{
  initRadios();
  initUltrasoic();
  initAverage();
  Serial.begin(9600);
}


int x = 0;
int previous = 0;
void loop()
{
  x++;

  
  if (x % 3 == 0) {
    /*
    unsigned int distanceOne = readDistanceOne();
    //unsigned int distanceTwo = readDistanceTwo();
    //sendRawData(distanceOne, distanceTwo);
    //if(distanceOne > previous + 1 || distanceOne < previous - 1){
      Serial.println(distanceOne);
      previous = distanceOne;
    //}
    */
    Serial.println(getAverageOne());
  }
  
  if (hasCarPassed()) {
    //sendCarData(getCarInformation());
    Serial.println("CAR HAS PASSED!!!!");
  }
  delay(20);
}
