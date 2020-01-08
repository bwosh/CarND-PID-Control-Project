#include <iostream>
#include "PID.h"

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  this->Kd = Kd_;
  this->Ki = Ki_;
  this->Kp = Kp_;

  this->cte_sum = 0;
  this->last_cte = 0;
}

SteetingData PID::getSteeringValueByPID(double cte, double speed, double angle)
{
  double dt_cte = cte-last_cte;
  cte_sum += cte;
  last_cte = cte;

  double steer = -Kp * cte - Kp*dt_cte - Ki*cte_sum ;

  std::cout << "CTE: " << cte << std::endl;
  std::cout << "speed: " << speed << std::endl;
  std::cout << "angle: " << angle << std::endl;
  std::cout << "Steering Value: " << steer << std::endl;

  SteetingData data;
  data.angle = steer;
  data.throttle = 0.1;

  return data;
}
