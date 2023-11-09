#pragma once

struct Job
{
    char type;
    int jobNumber;
    int arrivalTime;
    int processingTime;
};



class Processors
{

private:
    int numProcessors;
    Job* temp;

public:
	Job* processors[];

    ~Processors() { delete processors; *processors = nullptr; delete temp; temp = nullptr; } 

    Processors(int num);

    int getOpenProcessor();

    int getLeastTimeProcessor();

    bool allTypeD();

    int getNumProcessors() { return numProcessors; }

    Job* getTemp();



};

