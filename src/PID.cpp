#include "PID.h"

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  this->Kd = Kd_;
  this->Ki = Ki_;
  this->Kp = Kp_;
}

SteetingData PID::getSteeringValueByPID(double cte, double speed, double angle)
{
  SteetingData data;

  data.angle = 0;
  data.throttle = 0.3;

  return data;
}
