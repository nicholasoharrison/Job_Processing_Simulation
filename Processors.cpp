#include "Processors.h"


Processors::Processors(int num)
{
    processors[num];
    numProcessors = num;

    for (int i = 0; i < num; i++)
    {
        processors[i] = nullptr;
    }
    
}


int Processors::getOpenProcessor()
{
    for (int i = 0; i < numProcessors; i++)
    {
        if (processors[i] == nullptr)
        {
            return i;
        }
    }
    return -1;
}


int Processors::getLeastTimeProcessor()
{
    int least = 0;
    for (int i = 1; i < numProcessors; i++)
    {
        if (processors[i]->processingTime < processors[least]->processingTime)
        {
            least = i;
        }
    }

    return least;
}

bool Processors::allTypeD()
{
    for (int i = 0; i < numProcessors; i++)
    {
        if (processors[i]->type != 'D')
        {
            return false;
        }
    }

    return true;
}