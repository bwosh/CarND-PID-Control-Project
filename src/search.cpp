#include "search.h"
#include <iostream>

Search::Search(PID *pid, int iters_per_parameters, int all_loops)
{
    this->pid = pid;
    this->iters_per_parameters = iters_per_parameters;
    this->all_loops = all_loops;

    this->currentIteration = 0;
    this->currentLoop = 0;
}

void Search::nextIter()
{
    this->currentIteration++;
    if((this->currentIteration % this->iters_per_parameters)==0)
    {
        // TODO evaluate last loop
        this->currentLoop++;
        this->currentIteration = 0;
    }
    
    std::cout << this->currentIteration << std::endl;
    // TODO store error
}