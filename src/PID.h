#ifndef PID_H
#define PID_H


class PID {
private:

  /*
  * Errors
  */
  double p_error = 0;
  double i_error = 0;
  double d_error = 0;
  double iabs_error = 0;
  double i2_error = 0;
  int samples_count = 0;

  /*
  * Coefficients
  */
  double Kp;
  double Ki;
  double Kd;



public:
  /*
  * Constructor
  */
  PID(double Kp, double Kd, double Ki);

  /*
  * Destructor.
  */
  virtual ~PID();

 /*
  * Initialize coefficients
  */
  void Init(double Kp,  double Kd, double Ki);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();

  
  double GetKp();
  double GetKi();
  double GetKd();

  /*
  * Gets average error through whole run (from Init)
  */
  double MeanAbsoluteError();

  /*
  * Gets average error through whole run (from Init)
  */
  double MeanSquaredError();
};

#endif /* PID_H */