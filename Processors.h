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

public:
	Job* processors[];

    ~Processors() { delete processors; *processors = nullptr; }

    Processors(int num);

    int getOpenProcessor();

    int getLeastTimeProcessor();

    bool allTypeD();



};

