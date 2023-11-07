#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <queue>
#include "Processors.h"








void jobSort(std::vector<Job>& jobs)
{
    int n = jobs.size();
    for (int i = 0; i < n - 1; i++) 
    {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
        {
            if (jobs[j].arrivalTime < jobs[minIndex].arrivalTime ||
                (jobs[j].arrivalTime == jobs[minIndex].arrivalTime && jobs[j].type > jobs[minIndex].type)) 
            {
                minIndex = j;
            }
        }
        std::swap(jobs[minIndex], jobs[i]);
    }
}



int main() 
{


    std::vector<Job> jobs;
    std::default_random_engine generator;
    static int arrivalTimeA = 0;
    static int arrivalTimeB = 0;
    static int arrivalTimeC = 0;
    static int arrivalTimeD = 0;
    int overallJobNumber = 1; 

    // Type A jobs
    for (int i = 0; i < 2400; i++)
    {
        Job job;
        job.type = 'A';
        job.jobNumber = overallJobNumber++;
        job.arrivalTime = arrivalTimeA + 4 + rand() % 3;
        arrivalTimeA = job.arrivalTime;
        job.processingTime = 3 + rand() % 3;
        jobs.push_back(job);
    }

    // Type B jobs
    for (int i = 0; i < 1650; i++) 
    {
        Job job;
        job.type = 'B';
        job.jobNumber = overallJobNumber++;
        job.arrivalTime = arrivalTimeB + 6 + rand() % 3;
        arrivalTimeB = job.arrivalTime;
        job.processingTime = 5 + rand() % 3;
        jobs.push_back(job);
    }

    // Type C jobs
    for (int i = 0; i < 1100; i++)
    {
        Job job;
        job.type = 'C';
        job.jobNumber = overallJobNumber++;
        job.arrivalTime = arrivalTimeC + 11 + rand() % 2;
        arrivalTimeC = job.arrivalTime;
        job.processingTime = 9 + rand() % 3;
        jobs.push_back(job);
    }

    // Type D jobs
    for (int i = 0; i < 1200; i++) {
        Job job;
        job.type = 'D';
        job.jobNumber = overallJobNumber++;
        job.arrivalTime = arrivalTimeD + 9 + rand() % 6;
        arrivalTimeD = job.arrivalTime;
        job.processingTime = 12 + rand() % 11;
        jobs.push_back(job);
    }


    // Sort jobs
    jobSort(jobs);



    // Write jobs to a file
    std::ofstream outfile("jobs_data.txt");
    if (outfile.is_open()) 
    {
        for (const auto& job : jobs) 
        {
            outfile << job.type << " " << job.arrivalTime << " " << job.processingTime << "\n";
        }
        outfile.close();
        std::cout << "Data written to jobs_data.txt\n";
    }
    else 
    {
        std::cout << "Unable to open file\n";
    }

    std::vector<Job*> waitQueue;
    std::vector<Job> * temp = &jobs;

    int processorNum;
    std::cout << "\nEnter a number of processors: ";
    std::cin >> processorNum;

    

    Processors pro(processorNum);

    for (int i = 0; i < 500; i++)
    {
        std::cout << "\nTime " << i << ": ";
        if (jobs[i].arrivalTime == i)
        {
            if (pro.getOpenProcessor() > -1 && waitQueue.size() != 0)
            {
                //should put the top of the queue in one of the open proessors
            }
            if (pro.getOpenProcessor() > -1 && waitQueue.size() == 0)
            {
                pro.processors[pro.getOpenProcessor()] = &jobs[i];
            }
            else
            {
                if (jobs[i].type == 'D' && pro.allTypeD() != true)
                {
                    waitQueue.push_back(pro.processors[pro.getLeastTimeProcessor()]);
                    pro.processors[pro.getLeastTimeProcessor()] = &jobs[i];
                }
                else
                {
                    waitQueue.push_back(&jobs[i]);
                }
            }
        }
    }

    
   


   




    return 0;
}