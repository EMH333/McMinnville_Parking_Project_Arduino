#include <iostream>
#include "libraries/Average.h"
#include "exampleReadCSV.cpp"
#include "carFunction.cpp"
#include <cmath>

bool test(string dataFile);

int main(int, char **)
{
    //test("testFiles/data.csv");
    test("testFiles/generatedOneCar.csv");
    test("testFiles/Mon/one_good.txt");
}

//just use datafile to test
//TODO add time management
int currentTime = 0;
const int INCREASE = 30; //about 30 miliseconds every time
bool test(string dataFile)
{
    vector<vector<int>> vec = readData(dataFile);
    carFunction car;
    bool canUseRealTime = false;

    cout << "Starting test..." << endl;

    if (vec[1].size() == 3)
    {
        canUseRealTime = true;
        cout << "Using time from file" << endl;
    }

    unsigned int expectedCars = vec[0][0];
    unsigned int cars = 0;

    for (unsigned int i = 1; i < vec.size(); ++i)
    {
        if (canUseRealTime)
        {
            currentTime += vec[i][2];
        }
        else
        {
            currentTime += INCREASE;
        }

        if (car.hasCarPassed(vec[i][0], vec[i][1], currentTime))
        {
            cars++;
        }
    }

    cout << "Test with data from '" << dataFile;
    if (expectedCars == cars)
    {
        cout << "' was a sucess" << endl
             << endl;
        return true;
    }
    else
    {
        cout << "' FAILLED!" << endl;
        cout << "ERROR:                             EXPECTED: "
             << expectedCars << ", GOT: " << cars << " CARS!" << endl
             << endl;
        return false;
    }
}
