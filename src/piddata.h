#ifndef PIDDATA_H
#define PIDDATA_H

#include "PID.h"
#include "search.h"

class PIDData
{
    public:
        PIDData(PID *pid, Search *search);
        PID *GetPID();
        Search *GetSearch();
    private:
        PID *pid;
        Search *search;
};

#endif