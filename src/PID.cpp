#include "PID.h"

PID::PID(double Kp, double Ki, double Kd) {
  this->Init(Kp, Ki, Kd);
}

void PID::Init(double Kp, double Ki, double Kd)
{
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;

  d_error = 0;
  p_error = 0;
  i_error = 0;
}

PID::~PID() {}

void PID::UpdateError(double cte) {
  d_error = cte - p_error;
  p_error = cte;
  i_error += cte;
}

double PID::TotalError() {
  double steer = -Kp * p_error - Kd * d_error - Ki * i_error;

  // Check constraints
  if (steer < -1) {
    steer = -1;
  }
  if (steer > 1) {
    steer = 1;
  }

  return steer;
}
