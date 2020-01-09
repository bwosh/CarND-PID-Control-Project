#include "search.h"
#include <iostream>
#include <cmath>

Search::Search(PID *pid, int iters_per_parameters)
{
    this->pid = pid;
    this->iters_per_parameters = iters_per_parameters;

    this->currentIteration = 0;
    this->currentLoop = 0;

    // Set starting parameters
    this->Kp = pid->GetKp();
    this->Kd = pid->GetKd();
    this->Ki = pid->GetKi();

    this->bestError = std::numeric_limits<double>().max();
    this->saveBest(this->bestError, this->Kp, this->Kd, this->Ki );

    // Set twiddle initial values
    this->twiddle_index = 0;
    this->twiddle_direction = true;

    this->twiddle_values = (double **)(new void*[3]);
    this->twiddle_values[0] = &(this->Kp);
    this->twiddle_values[1] = &(this->Kd);
    this->twiddle_values[2] = &(this->Ki);

    this->twiddle_deltas[0] = this->Kp/2;
    this->twiddle_deltas[1] = this->Kd/2;
    this->twiddle_deltas[2] = this->Ki/2;

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
        bool new_best = false;

        if( error<this->bestError)
        {
            this->saveBest(error, Kp, Kd, Ki);
            new_best = true;
        }else{
            std::cout << " " << error << " is worse than best " <<this->bestError << ", Kp " << best_Kp
                << ", Kd:" << best_Kd
                << ", Ki:" << best_Ki
                << "@loop " << bestLoopIndex
                << std::endl;            
        }

        // Changing parameters anch starting next search loop
        this->changeParameters(new_best);
        this->currentLoop++;
        this->currentIteration = 0;
    }
}

void Search::saveBest(double err, double Kp, double Kd, double Ki)
{
    this->bestLoopIndex = this->currentLoop;
    this->bestError = err;
    this->best_Kp = Kp;
    this->best_Kd = Kd;
    this->best_Ki = Ki;

    std::cout << " [!!!] NEW BEST: " << err << " Kp " << Kp
            << ", Kd:" << Kd
            << ", Ki:" << Ki
            << std::endl;
}

void Search::changeParameters(bool newbest)
{
    if(newbest)
    {
        twiddle_deltas[twiddle_index] *= 1.1;
    }else{
        if(!twiddle_direction)
        {
            twiddle_deltas[twiddle_index] *= 0.9;   
        }
    }

    if(twiddle_direction)
    {
        *twiddle_values[twiddle_index] +=  twiddle_deltas[twiddle_index];
    }else{
        if(!newbest)
        {
            *twiddle_values[twiddle_index] -=  2*twiddle_deltas[twiddle_index];
        }
    }
    
    twiddle_direction = !twiddle_direction;
    if(!twiddle_direction)
        twiddle_index++;
    if(twiddle_index>=3)
        twiddle_index = 0; 
}