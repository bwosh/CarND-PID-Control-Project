#ifndef PID_H
#define PID_H

#include "search.h"

class PID {
private:

  /*
  * Errors
  */
  double p_error = 0;
  double i_error = 0;
  double d_error = 0;

  /*
  * Coefficients
  */
  double Kp;
  double Ki;
  double Kd;



public:
  Search *search; // TODO do private

  /*
  * Constructor
  */
  PID(Search *search);

  /*
  * Destructor.
  */
  virtual ~PID();

 /*
  * Initialize coefficients
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();
};

#endif /* PID_H */