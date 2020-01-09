#ifndef SEARCH_H
#define SEARCH_H

#include "pid.h"

class Search
{
    public:
        Search(PID *pid, int iters_per_parameters);
        void nextIter(void);
    private:
       void saveBest(double err, double Kp, double Kd, double Ki);
       void changeParameters(bool newbest);

       // Parameters to seach
       int iters_per_parameters;

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
       int bestLoopIndex;

       // Twiddle search
       double twiddle_deltas[3];
       double **twiddle_values;
       int twiddle_index;
       bool twiddle_direction;
};

#endif
