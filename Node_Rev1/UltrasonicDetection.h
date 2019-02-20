#ifndef ULTRASONIC_DETECTION
#define ULTRASONIC_DETECTION

#include "Arduino.h"

class CarInformation {
  public:
    CarInformation(uint8_t dir, unsigned long curTime);
    uint8_t getDirection();
    unsigned long getTime();
  private:
    uint8_t carDirection;//0=unknown direction, 1=in, 2=out
    unsigned long timeAtEvent;
};

//Returns true if a car has passed, this should trigger the run of additional logic once car is detected
bool hasCarPassed();

//Returns information of car at last event
CarInformation getCarInformation();

void initUltrasoic();


unsigned int readDistanceOne();//TEMP for TESTING
unsigned int readDistanceTwo();
#endif
