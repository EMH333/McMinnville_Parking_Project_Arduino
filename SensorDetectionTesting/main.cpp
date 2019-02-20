#include <iostream>
#include "exampleReadCSV.cpp"
#include "carFunction.cpp"
#include <cmath>

bool test(string dataFile);

int main(int, char **)
{
    //test("testFiles/data.csv");
    test("testFiles/generatedOneCar.csv");
}

//just use datafile to test
bool test(string dataFile)
{
    vector<vector<int>> vec = readData(dataFile);

    unsigned int expectedCars = vec[0][0];
    unsigned int cars = 0;

    for (unsigned int i = 1; i < vec.size(); ++i)
    {
        if (hasCarPassed(vec[i][0], vec[i][1]))
        {
            cars++;
        }
    }

    cout << "Test with data from '" << dataFile;
    if (expectedCars == cars)
    {
        cout << "' was a sucess" << endl;
        return true;
    }
    else
    {
        cout << "' FAILLED!" << endl;
        cout << "ERROR:                             EXPECTED: " 
        << expectedCars << ", GOT: " << cars << " CARS!" << endl;
        return false;
    }
}
