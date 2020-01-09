#include "piddata.h"

PIDData::PIDData(PID *pid, Search *search)
{
    this->pid = pid;
    this->search = search;
}

PID *PIDData::GetPID()
{
    return this->pid;
}

Search *PIDData::GetSearch()
{
    return this->search;
}