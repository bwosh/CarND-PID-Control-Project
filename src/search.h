#ifndef SEARCH_H
#define SEARCH_H

#include "pid.h"

class Search
{
    public:
        Search(PID *pid, int iters_per_parameters, int all_loops);
        void nextIter(void);
    private:
       void saveBest(double err, double Kp, double Kd, double Ki);
       void changeParameters(void);

       int iters_per_parameters;
       int all_loops;

       int currentLoop;
       int currentIteration; 

       PID *pid;
       double Kp;
       double Kd;
       double Ki;

       double bestError;
       double best_Kp;
       double best_Kd;
       double best_Ki;
};

#endif
