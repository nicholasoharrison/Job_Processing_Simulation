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

    //                       NOTE
    // I have the processors partly working. I need to implement the functionality of time taking away from the 
    // processing time of the jobs in the processors. I know how to do that but I just need a little more time.
    // The test plan I think should be started. I will leave more comments about the fuunctionality of the code
    // when i get a chance.
    // 
    // I am having an issue with possibly a memory leak so I placed a break point before the end of main to allow it to run
    // If you could figure that out it would be great
    //

    Processors pro(processorNum);

    for (int j = 0; j < 500; j++)
    {
        std::cout << "\nTime " << j << ":";
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
                        dQueue.push_back(&jobs[i]);
                        std::cout << "\n   " << jobs[i].type << ":" << jobs[i].jobNumber << " placed in queue...";
                    }
                }


            }
        }

        for (int i = 0; i < processorNum; i++)
        {
            
            if (pro.processors[i]->jobNumber > -1) // problem here when there is more than 3 processors - not sure why yet
            {
                (pro.processors[i]->processingTime)--;
                if (pro.processors[i]->processingTime == 0)
                {
                    std::cout << "\n   " << pro.processors[i]->type << ":" << pro.processors[i]->jobNumber << " DONE PROCESSING~~~";
                    pro.processors[i] = pro.getTemp();

                }
            }
        }
        

    }

    
   


   




    return 0;
}