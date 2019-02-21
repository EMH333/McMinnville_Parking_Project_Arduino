#include <iostream>
#include "libraries/Average.h"
#include "libraries/Average.cpp"
//
//
//
//THE IMPORTANT STUFF
//
//
//the function used to actually do the detection

#define MAX_CHANGE 250
#define MIN_CHANGE 140
#define MIN_TIME 750
int previousDistance = 0;
bool carPassing = false;
long carPassingTime = 0;
Average floorAverage(1500);
Average past(4);
bool hasCarPassed(int dOne, int dTwo, int currentTime)
{
    int currentDistance = dOne;
    past.addData(currentDistance);
    //std::cout << "Past: " << past.getAverage() << ", floor: " << floorAverage.getAverage()<< std::endl;
    if(carPassingTime == 0){
        floorAverage.addData(currentDistance);
    }
    bool passedCurrently = past.getAverage() < floorAverage.getAverage() - MIN_CHANGE;

    if (passedCurrently && !carPassing)
    {
        std::cout << "Current Time: " << currentTime << ", floor: " << floorAverage.getAverage() << ", past: " << past.getAverage() << std::endl;

        carPassing = true;
        carPassingTime = currentTime;
        return true;
    }
    else
    {
        if (carPassingTime + MIN_TIME < currentTime)
        {
            carPassing = false;
            previousDistance = currentDistance;
            floorAverage.addData(currentDistance); //add current distance to average
        }
    }
    return false;
}
