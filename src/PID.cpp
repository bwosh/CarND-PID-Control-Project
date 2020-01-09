#include "pid.h"
#include <cmath>
#include <iostream>

PID::PID(double Kp, double Kd, double Ki) {
  this->Init(Kp, Kd, Ki);
  this->p_errors_count = 0;
  this->p_errors_sum = 0;
  this->d_errors_count = 0;
  this->d_errors_sum = 0;
}

void PID::Init(double Kp, double Kd, double Ki)
{
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;

  d_error = 0;
  p_error = 0;
  i_error = 0;
  iabs_error = 0;
  i2_error = 0;
  samples_count = 0;
}

PID::~PID() {}

void PID::UpdateError(double cte) {
  d_error = cte - p_error;
  p_error = cte;
  i_error += cte;
  iabs_error += abs(cte);
  i2_error += cte*cte;
  samples_count++;

  GradientDescent();
}

double PID::MeanAbsoluteError()
{
  return iabs_error/(double)samples_count;
}

double PID::MeanSquaredError()
{
  return i2_error/(double)samples_count;
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

double PID::GetKp()
{
  return Kp;
}

double PID::GetKi()
{
  return Ki;
}

double PID::GetKd()
{
  return Kd;
}

void PID::GradientDescent(void){
  float paramP = 0.0001;        
  float paramD = 0.01;
  p_errors_count += 1;
  d_errors_count += 1;
  p_errors_sum += p_error;
  d_errors_sum += d_error;

  if (p_errors_count>=10000)
  {
    Kp = Kp - paramP * (p_errors_sum/p_errors_count);
    Kd = Kd - paramD * (d_errors_sum/d_errors_sum);

    std::cout << "[Kp " << Kp
              << ", Kd:" << Kd
              << ", Ki:" << Ki
              << "]" << std::endl;

    this->p_errors_count = 0;
    this->p_errors_sum = 0;
    this->d_errors_count = 0;
    this->d_errors_sum = 0;
  }

}