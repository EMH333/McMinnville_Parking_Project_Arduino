#include <iostream>
#include "libraries/Average.h"
#include "exampleReadCSV.cpp"
#include "carFunction.cpp"
#include <cmath>

bool test(string dataFile);
int passingTests = 0;
int totalTests = 0;
int main(int, char **)
{
    //test("testFiles/data.csv");
    //test("testFiles/generatedOneCar.csv");

    //good and verified tests
    test("testFiles/Mon/one-2.txt");
    test("testFiles/Mon/one_good.txt");
    test("testFiles/mon-bef/test3-s1ts2-good.txt");
    test("testFiles/mon-bef/test4-s2ts1.txt");
    test("testFiles/mon-bef/test6-s2ts1.txt");

    test("testFiles/alone/alone-test1.txt");
    test("testFiles/alone/alone-test2.txt");
    test("testFiles/alone/alone-test3.txt");

    //ify tests
    //test("testFiles/Thur/two-1-d2.txt");//This test is good only if the system relyes on the higher values consistantly, which it currently does not do
    test("testFiles/Thur/two-2-d1-good.txt");
    //test("testFiles/Mon/two-2.txt");//This test seems to be a little low. Perhaps might throw out

    //to sort
    //test("testFiles/mon-bef/test1-1c.txt");//one sensor goes wack after car goes through
    //test("testFiles/mon-bef/test2-s2ts1.txt");//all over the placeS
    //test("testFiles/mon-bef/test5-s1ts2.txt");
    //test("testFiles/mon-bef/test7-s1ts2.txt");

   cout << passingTests << " out of " << totalTests << " passed" << endl;
}

//just use datafile to test
//TODO include direction analysis
int currentTime = 0;
const int INCREASE = 30; //about 30 miliseconds every time
carFunction car;
bool test(string dataFile)
{
    vector<vector<int>> vec = readData(dataFile);
    bool canUseRealTime = false;
    vector<int> whenCarsDetected;

    cout << "Starting test..." << endl;
    totalTests++;

    currentTime += 1500; //add 1.5 seconds to the time to isolate each test but keep values going

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
        passingTests++;
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
