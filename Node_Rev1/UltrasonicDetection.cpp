#include "UltrasonicDetection.h"
#include "Arduino.h"
#include "runningAverage.h"

#define SENSOR_ONE_PIN 14//TBD
#define SENSOR_TWO_PIN 15//TBD
#define PULSE_TYPE HIGH
#define PULSE_TIMEOUT 1000 * 300//300 milisecond timeout

#define MICRO_PER_CM 58//58 microseconds per cm of distance

void initUltrasoic() {
  //going to use pulse in instead becaused most any pin can handle it
  pinMode(SENSOR_ONE_PIN, INPUT);//Init
  pinMode(SENSOR_TWO_PIN, INPUT);
}

#define MAX_CHANGE 250
#define MIN_CHANGE 130
#define MIN_TIME 750
int previousDistance = 0;
int ppreviousDistance = 0;//The second previous distance
bool carPassing = false;
long carPassingTime = 0;
bool hasCarPassed() {
  int currentDistance = readDistanceOne();
  bool passedCurrently = currentDistance < previousDistance - MIN_CHANGE;
  bool passedInTwo = currentDistance < ppreviousDistance - MIN_CHANGE;
  if ( (passedCurrently || passedInTwo )&& !carPassing) {
    carPassing = true;
    carPassingTime = millis();
    return true;
  } else {
    if (carPassingTime + MIN_TIME < millis()) {
      carPassing = false;
      ppreviousDistance = previousDistance;
      previousDistance = currentDistance;
      averageOne(currentDistance);//add current distance to average
    }
  }
  return false;
}

CarInformation getCarInformation() {
  return CarInformation(1, 142L); //temp example
}


unsigned int readDistanceOne() {
  /*unsigned long duration = pulseIn(SENSOR_ONE_PIN, PULSE_TYPE, PULSE_TIMEOUT);
    unsigned int distance = duration / MICRO_PER_CM;
    if(distance < 20){//less than 20 cm then return 0
    return 0;
    }else{
    return distance;
    }*/
  int raw = analogRead(SENSOR_ONE_PIN);//3.2mv per cm
  int voltage = map(raw, 0, 1023, 0, 3300);
  return (unsigned int) voltage / 3.2;
}

unsigned int readDistanceTwo() {
  int raw = analogRead(SENSOR_TWO_PIN);//3.2mv per cm
  int voltage = map(raw, 0, 1023, 0, 3300);
  return (unsigned int) voltage / 3.2;
}


//Car information declararation below this line
CarInformation::CarInformation(uint8_t dir, unsigned long curTime) {
  carDirection = dir;
  timeAtEvent = curTime;
}
