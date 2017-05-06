#include "PID.h"

/*
* PID CLASS
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd)  {
  this->Kp = Kp; // proportional coefficient
  this->Ki = Ki; // integral coefficient
  this->Kd = Kd; // differential coefficient

  i_error = 0.0; // integral cte
  p_error = 1.0;  // proportional cte
  d_error = 0.0; // differential cte
}

void PID::UpdateError(double cte) {
  // **The cross track error, cte is the current y position of the robot
  // (our reference is a horizontal line) along the x-axis.
  // To get the steering value we multiply the tau parameter with the cte.
  // We then call the move method which causes the robot to move based on the steer and speed values.
  // Add the x and y coordinates to the respective lists and then return them at the end.
  // first time through make the previous error equal to cte.
  d_error = cte - p_error; // diff_cte = cte - prev_cte -> p_error is equal to prev_cte
  p_error = cte; // update p_error
  i_error += cte; // add i_error
}

double PID::TotalError() {
  // -tau_p * cte - tau_d * diff_cte - tau_i * int_cte
  return -Kp * p_error - Kd * d_error - Ki * i_error;
}
