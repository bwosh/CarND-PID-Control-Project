#ifndef PID_H
#define PID_H

struct SteetingData
{
    double angle;
    double throttle;
};

class PID {
 public:
  /**
   * Constructor
   */
  PID();

  /**
   * Destructor.
   */
  virtual ~PID();

  /**
   * Initialize PID.
   * @param (Kp_, Ki_, Kd_) The initial PID coefficients
   */
  void Init(double Kp_, double Ki_, double Kd_);

  /**
   * Steering calculation
   */
  SteetingData getSteeringValueByPID(double cte, double speed, double angle);

 private:
  /**
   * PID Coefficients
   */ 
  double Kp;
  double Ki;
  double Kd;

  double cte_sum;
  double last_cte;
};

#endif  // PID_H