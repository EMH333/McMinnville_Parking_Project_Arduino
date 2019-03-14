#include <iostream>
#include "libraries/Average.h"
#include "exampleReadCSV.cpp"
#include "carFunction.cpp"
#include <cmath>

bool test(string dataFile);

int main(int, char **)
{
    //test("testFiles/data.csv");
    //test("testFiles/generatedOneCar.csv");

    //good and verified tests
    test("testFiles/Mon/one-2.txt");
    test("testFiles/Mon/two-2.txt");
    test("testFiles/Mon/one_good.txt");

    //ify tests
    test("testFiles/Thur/two-1-d2.txt");
    test("testFiles/Thur/two-2-d1-good.txt");
}

//just use datafile to test
//TODO include direction analysis
int currentTime = 0;
const int INCREASE = 30; //about 30 miliseconds every time
bool test(string dataFile)
{
    vector<vector<int>> vec = readData(dataFile);
    carFunction car;
    bool canUseRealTime = false;
    vector<int> whenCarsDetected;

    cout << "Starting test..." << endl;

    currentTime = 0; //reset time

    //if there is a timing colum
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
            whenCarsDetected.push_back(i);
        }
    }

    if (expectedCars == cars)
    {
        cout << "Test with data from '" << dataFile << "' was a sucess" << endl
             << endl;
        return true;
    }
    else
    {
        for (int i = 0; i < whenCarsDetected.size(); i++)
        {
            cout << "Car detected at: " << whenCarsDetected[i] << endl;
        }

        char esc_char = 27; // the decimal code for escape character is 27
        cout << "Test with data from '" << dataFile << "' FAILLED!" << endl;
        cout << esc_char << "[1m"<< esc_char << "[31m"
             << "ERROR:                             EXPECTED: "
             << expectedCars << " CARS, GOT: " << cars << " CARS!" << endl
             << esc_char << "[0m" << endl;
        return false;
    }
}
