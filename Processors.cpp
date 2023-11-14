#include "Processors.h"



Processors::Processors(int num)
{
    processors[num];
    numProcessors = num;
    temp = new Job;
    temp->jobNumber = -1;
    for (int i = 0; i < num; i++)
    {
        processors[i] = temp;  
    }
    
}


int Processors::getOpenProcessor()
{
    
    
    for (int i = 0; i < numProcessors; i++)
    {
        if (processors[i]->jobNumber == -1) 
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
        if ((processors[i]->processingTime < processors[least]->processingTime) && processors[i]->type != 'D')
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



Job* Processors::getTemp()
{
    return temp;
}


bool Processors::isEmpty()
{
    for (int i = 0; i < numProcessors; i++)
    {
        if (processors[i]->jobNumber > 0) 
        {
            return false;
        }
    }

    return true;
}