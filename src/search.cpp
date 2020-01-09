#include "search.h"
#include <iostream>
#include <cmath>

Search::Search(PID *pid, int iters_per_parameters, int validate_every_n_loops)
{
    this->pid = pid;
    this->iters_per_parameters = iters_per_parameters;
    this->validate_every_n_loops = validate_every_n_loops;

    this->currentIteration = 0;
    this->currentLoop = 0;

    // Set starting parameters
    this->Kp = pid->GetKp();
    this->Kd = pid->GetKd();
    this->Ki = pid->GetKi();

    this->saveBest(std::numeric_limits<double>().max(), this->Kp, this->Kd, this->Ki );

    // Set twiddle initial values
    this->twiddle_index = 0;
    this->twiddle_direction = true;

    this->twiddle_values = (double **)(new void*[3]);
    this->twiddle_values[0] = &(this->Kp);
    this->twiddle_values[1] = &(this->Kd);
    this->twiddle_values[2] = &(this->Ki);

    static double twiddle_initial_factor =  1.0/50.0;

    this->twiddle_deltas[0] = this->Kp*twiddle_initial_factor;
    this->twiddle_deltas[1] = this->Kd*twiddle_initial_factor;
    this->twiddle_deltas[2] = this->Ki*twiddle_initial_factor;
}

void Search::nextIter()
{
    if(this->currentIteration==0)
    {
        std::cout << "===== Loop " << this->currentLoop 
                << "  [Kp: " << Kp
                << ", Kd:" << Kd
                << ", Ki:" << Ki
                << "]"
                << std::endl;
    }

    this->currentIteration++;
    
    if((this->currentIteration % this->iters_per_parameters)==0)
    {        
        // Handling new error and saving best result if applicable
        double error = pid->MeanSquaredError();
        bool new_best = false;

        if( error<this->bestError)
        {
            this->saveBest(error, Kp, Kd, Ki);
            new_best = true;
        }else{
            std::cout << " " << error << " is worse than best " <<this->bestError << ", [Kp " << best_Kp
                << ", Kd:" << best_Kd
                << ", Ki:" << best_Ki
                << "] @loop " << bestLoopIndex
                << std::endl;            
        }

        // Changing parameters anch starting next search loop
        this->changeParameters(new_best);
        this->currentLoop++;
        this->currentIteration = 0;

        if( (this->currentLoop % validate_every_n_loops) == 0)
        {
            std::cout << " Performing validation of the best model." <<  std::endl;
            this->Kp = this->best_Kp;
            this->Kd = this->best_Kd;
            this->Ki = this->best_Ki;
            pid->Init(this->Kp, this->Kd, this->Ki);
            this->saveBest(std::numeric_limits<double>().max(), this->Kp, this->Kd, this->Ki );
        }
    }
}

void Search::saveBest(double err, double Kp, double Kd, double Ki)
{
    this->bestLoopIndex = this->currentLoop;
    this->bestError = err;
    this->best_Kp = Kp;
    this->best_Kd = Kd;
    this->best_Ki = Ki;

    std::cout << " [!!!] NEW BEST: " << err << " [Kp " << Kp
            << ", Kd:" << Kd
            << ", Ki:" << Ki
            << "]" << std::endl;
}

void Search::changeParameters(bool newbest)
{
    static double multiply_decay = 0.1;

    if(newbest)
    {
        twiddle_deltas[twiddle_index] *= 1.0+multiply_decay;
    }else{
        if(!twiddle_direction)
        {
            twiddle_deltas[twiddle_index] *= 1.0-multiply_decay;   
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

    double deltas_sum = 0;
    for(int i=0;i<3;++i)
        deltas_sum += twiddle_deltas[i];
    std::cout << " Twiddle deltas sum:" << deltas_sum << std::endl;
    
    if(!twiddle_direction)
        twiddle_index++;
    if(twiddle_index>=3)
        twiddle_index = 0; 

    twiddle_direction = !twiddle_direction;

    pid->Init(this->Kp, this->Kd, this->Ki);
}