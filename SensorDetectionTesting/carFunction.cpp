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
#define MIN_CHANGE 135
#define MIN_TIME_TO_PASS 750
#define MIN_TIME_BETWEEN_CARS 200
#define MAX_SENSOR_ERROR 20 //the max error before we default to sensor one
#define SENSOR_ERROR_DISTANCE 255
#define SENSOR_ERROR_TOLERANCE 5 //how far around sensor error distance where we think sensor is broken
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
    void resetDirection(); //called when the system knows a car is not passing to reset for next car
    int sOneMin = INT16_MAX;
    int sTwoMin = INT16_MAX;
    int sOneMinTime = 0;
    int sTwoMinTime = 0;

  public:
    bool hasCarPassed(int dOne, int dTwo, int currentTime);
    int getDirection(int time, int error); //gets direction of most recent car, with the specified time and acceptable error
};

bool carFunction::hasCarPassed(int dOne, int dTwo, int currentTime)
{
    int currentDistance;
    if (abs(dOne - dTwo) < MAX_SENSOR_ERROR)
    {
        currentDistance = (dOne + dTwo) / 2; //if close use current distance as an average
    }
    else
    {   //by default pick the sensor reading that isn't close to 255 and is the highest not above 500 (15 feet which we should never reach)
        //and not lower than 30 (should never reach)
        if (dOne > dTwo)
        {
            if (dOne > SENSOR_ERROR_DISTANCE + SENSOR_ERROR_TOLERANCE && dOne < SENSOR_ERROR_DISTANCE - SENSOR_ERROR_TOLERANCE && dOne < 500 && dOne > 30)
            {
                currentDistance = dOne;
            }
            else
            {
                currentDistance = dTwo; //TODO currently accepting this blindly, might need to modify
            }
        }
        else
        {
            if (dTwo > SENSOR_ERROR_DISTANCE + SENSOR_ERROR_TOLERANCE && dTwo < SENSOR_ERROR_DISTANCE - SENSOR_ERROR_TOLERANCE && dTwo < 500 && dTwo > 30)
            {
                currentDistance = dTwo;
            }
            else
            {
                currentDistance = dOne; //TODO currently accepting this blindly, might need to modify
            }
        }
    }

    updateDirection(dOne, dTwo, currentTime);

    past.addData(currentDistance);
    //std::cout << "Past: " << past.getAverage() << ", floor: " << floorAverage.getAverage()
    //<< ", difference: " << floorAverage.getAverage() - past.getAverage() << std::endl;
    if (carPassingTime == 0)
    {
        floorAverage.addData(currentDistance);
    }

    //test if there has been a significant change, that might signify a car
    bool passedCurrently = floorAverage.getAverage() - past.getAverage() >= MIN_CHANGE;

    //if detecting a car, a car not currently passing and there has been enough time between cars, then trigger
    if (passedCurrently && !carPassing && currentTime > carPassingTime + MIN_TIME_BETWEEN_CARS)
    {
        std::cout << "Current Time: " << currentTime << ", floor: " << floorAverage.getAverage()
                  << ", past average: " << past.getAverage()
                  << ", difference: " << floorAverage.getAverage() - past.getAverage()
                  << ", direction: " << getDirection(currentTime, 100) << std::endl;

        carPassing = true;
        carPassingTime = currentTime;
        return true;
    }
    else
    {
        //if data has setled and no car is passing, update distances and go back to monitoring floor
        if (carPassingTime + MIN_TIME_TO_PASS < currentTime)
        {
            if (carPassing)
            { //if currently car passing, we know this is the first time after a car has passed this part has run
                resetDirection();
            }
            carPassing = false;
            previousDistance = currentDistance;
            floorAverage.addData(currentDistance); //add current distance to average
        }
    }
    return false;
}

void carFunction::updateDirection(int dOne, int dTwo, int currentTime)
{
    //there are a few ways to do this
    //1. which sensor got low first//in
    //2. which sensor got high first//out

    //x^(2/3)

    if (abs(dOne - sOneMin) > pow(dOne, 2.0 / 3))
    {
        sOneMin = dOne;
        sOneMinTime = currentTime;
    }

    if (abs(dTwo - sTwoMin) > pow(dTwo, 2.0 / 3))
    {
        sTwoMin = dTwo;
        sTwoMinTime = currentTime;
    }

    if (sTwoMinTime > sOneMinTime)
    {
        bestDirectionGuess = 1;
        bestDirectionTime = currentTime;
    }
    else
    {
        bestDirectionGuess = 2;
        bestDirectionTime = currentTime;
    }
}

void carFunction::resetDirection()
{
    int bestDirectionGuess = -1;
    int bestDirectionTime = 0;

    int sOneMin = INT16_MAX;
    int sTwoMin = INT16_MAX;
    int sOneMinTime = 0;
    int sTwoMinTime = 0;
}

int carFunction::getDirection(int askTime, int error)
{
    //perhaps use the average guess of the last say 5 or 10 readings
    if (askTime - error < bestDirectionTime && bestDirectionGuess != -1)
    {
        return bestDirectionGuess;
    }
    else
    {
        return -1;
    }
}