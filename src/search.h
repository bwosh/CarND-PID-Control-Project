#ifndef SEARCH_H
#define SEARCH_H

#include "pid.h"

class Search
{
    public:
        Search(PID *pid, int iters_per_parameters, int all_loops);
        void nextIter();
    private:
       int iters_per_parameters;
       int all_loops;
       int currentLoop;
       int currentIteration; 
       PID *pid;
};

#endif
