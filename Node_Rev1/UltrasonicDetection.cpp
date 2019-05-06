#include "UltrasonicDetection.h"
#include "Arduino.h"
#include "Average.h"


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

unsigned int readDistanceOne() {
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

CarInformation getCarInformation() {
  return CarInformation(millis()%3, 142L); //temp example
}

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
#define MAX_SENSOR_ERROR 20 //the max error before the system does math to figure out the most likely good sensor
#define SENSOR_ERROR_DISTANCE 255
#define SENSOR_ERROR_TOLERANCE 5 //how far around sensor error distance where we think sensor is broken
#define FLOOR_AVERAGE_SIZE 2000
int previousDistance = 0;
bool carPassing = false;
long carPassingTime = 0;
Average floorAverage = Average(FLOOR_AVERAGE_SIZE);
Average past = Average(4);

int bestDirectionGuess = -1;
int bestDirectionTime = 0;
int sOneMin = INT16_MAX;
int sTwoMin = INT16_MAX;
int sOneMinTime = 0;
int sTwoMinTime = 0;
Average sTwoDirectionOffset = Average(FLOOR_AVERAGE_SIZE);

bool hasCarPassed(int dOne, int dTwo, int currentTime)
{
    int currentDistance;
    //This portion of the code finds the distance to use when tracking the cars
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

    updateDirection(dOne, dTwo, currentTime);//update the direction guess

    past.addData(currentDistance);
    if (carPassingTime == 0)
    {
        floorAverage.addData(currentDistance);
    }


    //if the current readings are below the current floor average, weight the floor down a bit more
    if(past.getAverage() > floorAverage.getAverage()){
        int av = past.getAverage();
        floorAverage.addData(av);
    }

    //test if there has been a significant change, that might signify a car
    bool passedCurrently = floorAverage.getAverage() - past.getAverage() >= MIN_CHANGE;

    //if detecting a car, a car not currently passing and there has been enough time between cars, then trigger
    if (passedCurrently && !carPassing && currentTime > carPassingTime + MIN_TIME_BETWEEN_CARS)
    {
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

//TODO add a confidence level
void updateDirection(int dOne, int dTwo, int currentTime)
{
    //there are a few ways to do this
    //1. which sensor got low first//in
    //2. which sensor got high first//out
    //x^(2/3) provides the correct number for a significant change
    //if s2 is higher in the beginning and lower at the end, d1

    //overcomes sensor measurement offset when using same surface
    sTwoDirectionOffset.addData(dOne - dTwo);
    dTwo += sTwoDirectionOffset.getAverage();

    //the abs and pow in the if statments is checking if there has been significant change and updating accordingly
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

void resetDirection()
{
    int bestDirectionGuess = -1;
    int bestDirectionTime = 0;

    int sOneMin = INT16_MAX;
    int sTwoMin = INT16_MAX;
    int sOneMinTime = 0;
    int sTwoMinTime = 0;
}

int getDirection(int askTime, int error)
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
