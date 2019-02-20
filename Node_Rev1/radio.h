#ifndef RADIO_CONTROL
#define RADIO_CONTROL

#include "Arduino.h"
#include "UltrasonicDetection.h"

void initRadios();
void oldLoop();
bool sendCarData(CarInformation car);
void sendRawData(uint8_t one, uint8_t two);//just for test
#endif
