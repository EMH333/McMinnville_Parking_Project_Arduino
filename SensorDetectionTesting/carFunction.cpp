#include <iostream>
#include <cmath>
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
#define MIN_CHANGE 150
#define MIN_TIME_TO_PASS 750
#define MIN_TIME_BETWEEN_CARS 200
#define MAX_SENSOR_ERROR 20 //the max error before we default to sensor one
class carFunction
{
  private:
    int previousDistance = 0;
    bool carPassing = false;
    long carPassingTime = 0;
    Average floorAverage = Average(1500);
    Average past = Average(4);

    int bestDirectionGuess = -1;
    int bestDirectionTime = 0;
    void updateDirection(int dOne, int dTwo, int currentTime);
    void resetDirection();//called when the system knows a car is not passing to reset for next car

  public:
    bool hasCarPassed(int dOne, int dTwo, int currentTime);
    int getDirection(int time, int error);//gets direction of most recent car, with the specified time and acceptable error
};

bool carFunction::hasCarPassed(int dOne, int dTwo, int currentTime)
{
    int currentDistance;
    if (abs(dOne - dTwo) < MAX_SENSOR_ERROR)
    {
        currentDistance = (dOne + dTwo) / 2; //use current distance as an average
    }
    else
    { //by default
        currentDistance = dOne;
    }

    past.addData(currentDistance);
    //std::cout << "Past: " << past.getAverage() << ", floor: " << floorAverage.getAverage()<< std::endl;
    if (carPassingTime == 0)
    {
        floorAverage.addData(currentDistance);
    }

    //test if there has been a significant change, that might signify a car
    bool passedCurrently = past.getAverage() < floorAverage.getAverage() - MIN_CHANGE;

    //if detecting a car, a car not currently passing and there has been enough time between cars, then trigger
    if (passedCurrently && !carPassing && currentTime > carPassingTime + MIN_TIME_BETWEEN_CARS)
    {
        std::cout << "Current Time: " << currentTime << ", floor: " << floorAverage.getAverage() << ", past: " << past.getAverage() << std::endl;

        carPassing = true;
        carPassingTime = currentTime;
        return true;
    }
    else
    {
        //if data has setled and no car is passing, update distances and go back to monitoring floor
        if (carPassingTime + MIN_TIME_TO_PASS < currentTime)
        {
            carPassing = false;
            previousDistance = currentDistance;
            floorAverage.addData(currentDistance); //add current distance to average
        }
    }
    return false;
}



void carFunction::updateDirection(int dOne, int dTwo, int currentTime)
{

}

void carFunction::resetDirection(){

}

int carFunction::getDirection(int time, int error){
    
}