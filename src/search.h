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

       // Parameters to seach
       int iters_per_parameters;
       int all_loops;

       // Variables to save current search state
       int currentLoop;
       int currentIteration; 

       // Reference t PID controller
       PID *pid;

       // Current parameters
       double Kp;
       double Kd;
       double Ki;

       // Best parameters
       double bestError;
       double best_Kp;
       double best_Kd;
       double best_Ki;
};

#endif
