#include "runningAverage.h"

const int numReadings = 1000;

int readingsOne[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
bool hasLooped = false; //only true if completed atleast one loop

void initAverage() {
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readingsOne[thisReading] = 0;
  }
}

void averageOne(int input) {
  // subtract the last reading:
  total = total - readingsOne[readIndex];
  // read from the sensor:
  readingsOne[readIndex] = input;
  // add the reading to the total:
  total = total + readingsOne[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  if (readIndex > numReadings - 3) {
    hasLooped = true;
  }
}

int getAverageOne() {
  if (hasLooped) {
    average = total / numReadings;
  } else {
    average = total / readIndex;
  }

  return average;
}


