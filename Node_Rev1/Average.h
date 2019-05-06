#ifndef AVERAGE
#define AVERAGE

class Average{
public:
    Average(int N);
    void addData(int input);
    int getAverage();
private:
    int data[2000];//this is a little big but I don't want to take the time to optimize it
    int numReadings = 0;
    int readIndex = 0;              // the index of the current reading
    int total = 0;                  // the running total
    int average = 0;                // the average
    bool hasLooped;         // has completed one full loop and now overriding?
};

#endif
