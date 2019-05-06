#include "Average.h"

Average::Average(int s)
{
  numReadings = s;
  //data = Vector(numReadings);
  for (int i = 0; i < numReadings; i++)
  {
    data[i] = 0;
  }
}

void Average::addData(int input)
{
  // subtract the last reading:
  total = total - data[readIndex];
  // read from the sensor:
  data[readIndex] = input;
  // add the reading to the total:
  total = total + data[readIndex];
  // advance to the next position in the array:
  readIndex++;

  // if we're at the end of the array...
  if (readIndex >= numReadings)
  {
    // ...wrap around to the beginning:
    readIndex = 0;
    hasLooped = true;
  }
}

int Average::getAverage()
{
  if (hasLooped)
  {
    average = total / numReadings;
  }
  else
  {
    if (readIndex != 0)
    {
      average = total / readIndex;
    }
    else
    {
      average = 1;
    }
  }

  return average;
}
