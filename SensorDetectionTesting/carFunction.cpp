const int DATA_SIZE = 100;
int floorVal = 500; //the value of where the floor is (will be subsitubed for average)
int aData[DATA_SIZE];
int counter = 0;
int average = floorVal; //start with average at floor val to compinstate
bool firstRound = true;
int addData(int val)
{
    aData[counter] = val;
    counter++;
    if (counter >= DATA_SIZE)
    {
        firstRound = false;
        counter = 0;
    }

    int sum = 0;
    for (int i = 0; i < DATA_SIZE; i++)
    {
        sum += aData[i];
    }

    average = sum / DATA_SIZE;
    if(firstRound){
        average = sum / counter;
    }
    //cout << "sum: " << sum << ", AVERAGE: " << average << endl;
    return average;
}

//
//
//
//THE IMPORTANT STUFF
//
//
//the function used to actually do the detection

int minDiff = 150;  //minimum difference to trigger system
bool carPassing = false;
int previous = average;
bool hasCarPassed(int dOne, int dTwo)
{
    if(dOne >= average){
        addData(dOne);
        carPassing = false;
        return false;
    }
    if ((dOne < average - minDiff) || (previous < average - minDiff))
    {
        previous = dOne;
        if (!carPassing)
        {
            carPassing = true;
            cout << "Detected car with distance: " << dOne << ", Average: " << average << endl;
            return true;
        }else{
            addData(dOne);
            return false;
        }
    }else{
        previous = dOne;
        carPassing = false;
        addData(dOne);
        return false;
    }
}


