#include "search.h"
#include <iostream>
#include <cmath>

Search::Search(PID *pid, int iters_per_parameters, int all_loops)
{
    this->pid = pid;
    this->iters_per_parameters = iters_per_parameters;
    this->all_loops = all_loops;

    this->currentIteration = 0;
    this->currentLoop = 0;

    // Set starting parameters
    this->Kp = pid->GetKp();
    this->Kd = pid->GetKd();
    this->Ki = pid->GetKi();

    this->bestError = std::numeric_limits<double>().max();
    this->saveBest(this->bestError, this->Kp, this->Kd, this->Ki );
}

void Search::nextIter()
{
    this->currentIteration++;
    if((this->currentIteration % this->iters_per_parameters)==0)
    {
        double mae = pid->MeanAbsoluteError();
        double mse = pid->MeanSquaredError();

        // Debug info
        std::cout << "===== Loop " << this->currentLoop 
                << ", Iter:" << this->currentIteration
                << " =====" << std::endl;

        std::cout << " Kp " << Kp
                << ", Kd:" << Kd
                << ", Ki:" << Ki
                << std::endl;

        std::cout << " MAE:" << mae
                << ", MSE:" << mse
                << ", sqrt(MSE):" << sqrt(mse)
                << std::endl;
        
        // Handling new error and saving best result if applicable
        double error = mse;

        if( error<this->bestError)
        {
            this->saveBest(error, Kp, Kd, Ki);
        }else{
            std::cout << " " << error << " is worse than best " <<this->bestError << ", Kp " << Kp
                << ", Kd:" << Kd
                << ", Ki:" << Ki
                << std::endl;            
        }

        // Changing parameters anch starting next search loop
        this->changeParameters();
        this->currentLoop++;
        this->currentIteration = 0;
    }
}

void Search::saveBest(double err, double Kp, double Kd, double Ki)
{
    this->bestError = err;
    this->best_Kp = Kp;
    this->best_Kd = Kd;
    this->best_Ki = Ki;

    std::cout << " [!!!] NEW BEST: " << err << " Kp " << Kp
            << ", Kd:" << Kd
            << ", Ki:" << Ki
            << std::endl;
}

void Search::changeParameters()
{
    // TODO change parameters algorithmically
    //std::cout << "...TODO:CHANGE..." << std::endl;
}