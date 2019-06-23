#include "UltrasonicDetection.h"
#include "radio.h"
//#include "runningAverage.h"

#define HEARTBEAT_INTERVAL 10000//10 seconds between heartbeats (for now)

void setup()
{
  initRadios();
  initUltrasoic();
  //initAverage();
  Serial.begin(9600);
}

unsigned long previousHeartbeat = 0;
void loop()
{
  if(hasCarPassed(readDistanceOne(),readDistanceTwo(),millis())){
    Serial.println("CAR PASSED");
    sendCarData(getCarInformation());
  }

  if(abs(millis() - previousHeartbeat) > HEARTBEAT_INTERVAL){
    sendHeartbeat(readDistanceOne(), readDistanceTwo());
    previousHeartbeat = millis();
  }
}
