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
    std::vector<Job*> dQueue;
    std::vector<Job> * temp = &jobs;

    int processorNum;
    std::cout << "\nEnter a number of processors: ";
    std::cin >> processorNum;

    int qFront = 0;
    int dqFront = 0;

    int avgQueueSize = 0;
    int qFullTime = 0;

    int cpuFullTime = 0;
    int time = 0;

    int aCompleted = 0;
    int bCompleted = 0;
    int cCompleted = 0;
    int dCompleted = 0;
    int totalCompleted = 0;

    int aArrived = 0;
    int bArrived = 0;
    int cArrived = 0;
    int dArrived = 0;

    

    Processors pro(processorNum); // weird error with processorNum = 2, 4, 5

    for (int j = 0; j < 500; j++)
    {
        std::cout << "\nTime " << j << ":";
        if (!pro.isEmpty()) { cpuFullTime++; }
        time++;
        if (pro.getOpenProcessor() > -1 && (waitQueue.size() != 0 || dQueue.size() != 0))
        {
            
            if (dQueue.size() == 0)
            {
                pro.processors[pro.getOpenProcessor()] = waitQueue[qFront];
                std::cout << "\n   " << waitQueue[qFront]->type << ":" << waitQueue[qFront]->jobNumber << " processing... ";
                qFront++;
            }
            else
            {
                pro.processors[pro.getOpenProcessor()] = dQueue[dqFront];
                std::cout << "\n   " << dQueue[dqFront]->type << ":" << dQueue[dqFront]->jobNumber << " processing... ";
                dqFront++;
            }
        }
        for (int i = 0; i < 500; i++)
        {     
            if (jobs[i].arrivalTime == j)
            {
                if (jobs[i].type == 'A') { aArrived++; }
                if (jobs[i].type == 'B') { bArrived++; }
                if (jobs[i].type == 'C') { cArrived++; }
                if (jobs[i].type == 'D') { dArrived++; }
                
                if (pro.getOpenProcessor() > -1 && waitQueue.size() == 0 && dQueue.size() == 0)
                {
                    pro.processors[pro.getOpenProcessor()] = &jobs[i];
                    std::cout << "\n   " << jobs[i].type << ":" << jobs[i].jobNumber << " processing... ";
                }
                else
                {
                    
                    if (jobs[i].type == 'D' && pro.allTypeD() != true)
                    {
                        waitQueue.push_back(pro.processors[pro.getLeastTimeProcessor()]);
                        std::cout << "\n   "  << pro.processors[pro.getLeastTimeProcessor()]->type << ":" << pro.processors[pro.getLeastTimeProcessor()]->jobNumber << " placed in queue...";
                        pro.processors[pro.getLeastTimeProcessor()] = &jobs[i];
                        std::cout << "\n   " << jobs[i].type << ":" << jobs[i].jobNumber << " processing... ";
                    }
                    else
                    {
                        if(jobs[i].type == 'D')
                        {
                            dQueue.push_back(&jobs[i]);
                        }
                        else
                        {
                            waitQueue.push_back(&jobs[i]);
                        }
                        
                        std::cout << "\n   " << jobs[i].type << ":" << jobs[i].jobNumber << " placed in queue...";
                    }
                }


            }
        }

        for (int i = 0; i < processorNum; i++)
        {
            
            if (pro.processors[i]->jobNumber > -1) 
            {
                (pro.processors[i]->processingTime)--;
                if (pro.processors[i]->processingTime == 0)
                {
                   
                    if (pro.processors[i]->type == 'A') { aCompleted++; }
                    if (pro.processors[i]->type == 'B') { bCompleted++; }
                    if (pro.processors[i]->type == 'C') { cCompleted++; }
                    if (pro.processors[i]->type == 'D') { dCompleted++; }

                    

                    totalCompleted++;
                    
                    std::cout << "\n   " << pro.processors[i]->type << ":" << pro.processors[i]->jobNumber << " DONE PROCESSING~~~";
                    pro.processors[i] = pro.getTemp();

               
                }
            }
        }

        avgQueueSize = avgQueueSize + (waitQueue.size() + dQueue.size() - qFront - dqFront);

        if (dQueue.size() + waitQueue.size() != 0)
        {
            qFullTime++;
        }
        
        

    }

    avgQueueSize /= 500.00;

    
    std::cout << "\n\n\n\nNumber of processors: " << processorNum;
    std::cout << "\nCurrent queue size: " << waitQueue.size() + dQueue.size() - qFront - dqFront;
    std::cout << "\nAverage queue size: " << avgQueueSize;
    std::cout << "\nMax queue size: " << waitQueue.size() + dQueue.size();
    std::cout << "\nTotal time jobs are in queue: " << qFullTime;
    std::cout << "\nAverage time jobs are in queue: "; // not sure how to implement this yet
    std::cout << "\nTotal number of A jobs arrived: " << aArrived;
    std::cout << "\nTotal number of A jobs completed: " << aCompleted;
    std::cout << "\nTotal number of B jobs arrived: " << bArrived;
    std::cout << "\nTotal number of B jobs completed: " << bCompleted;
    std::cout << "\nTotal number of C jobs arrived: " << cArrived;
    std::cout << "\nTotal number of C jobs completed: " << cCompleted;
    std::cout << "\nTotal number of D jobs arrived: " << dArrived;
    std::cout << "\nTotal number of D jobs completed: " << dCompleted;
    std::cout << "\nTotal jobs completed: " << totalCompleted;
    std::cout << "\nTotal time CPU(s) were processing: " << cpuFullTime;
    std::cout << "\nTotal time CPU(s) were idle: " << time - cpuFullTime;


   




    return 0;
}