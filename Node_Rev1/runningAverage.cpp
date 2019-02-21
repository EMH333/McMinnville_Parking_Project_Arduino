#include "runningAverage.h"


Average::Average(int size) {
  numReadings = size;
  hasLooped = false;
  data.resize(numReadings);
}

void Average::addData(int input) {
  // subtract the last reading:
  total = total - data[readIndex];
  // read from the sensor:
  data[readIndex] = input;
  // add the reading to the total:
  total = total + data[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  if (readIndex > numReadings - 1) {
    hasLooped = true;
  }
}

int Average::getAverage() {
  if (hasLooped) {
    average = total / numReadings;
  } else {
    average = total / readIndex;
  }

  return average;
}


