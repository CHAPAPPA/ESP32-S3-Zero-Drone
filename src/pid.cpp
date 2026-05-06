#include "pid.h"

void PID_Init(PID_t *pid, float kp, float ki, float kd,
              float integral_limit, float output_limit)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->integral = 0.0f;
    pid->prev_error = 0.0f;

    pid->integral_limit = integral_limit;
    pid->output_limit = output_limit;
}

void PID_Reset(PID_t *pid)
{
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
}

float PID_Update(PID_t *pid, float setpoint, float measurement, float dt)
{
    float error = setpoint - measurement;

    // Proportional
    float P = pid->kp * error;

    // Integral (anti-windup)
    pid->integral += error * dt;

    if (pid->integral > pid->integral_limit)
        pid->integral = pid->integral_limit;
    else if (pid->integral < -pid->integral_limit)
        pid->integral = -pid->integral_limit;

    float I = pid->ki * pid->integral;

    // Derivative
    float derivative = (error - pid->prev_error) / dt;
    float D = pid->kd * derivative;

    pid->prev_error = error;

    // Output
    float output = P + I + D;

    // Clamp output
    if (output > pid->output_limit)
        output = pid->output_limit;
    else if (output < -pid->output_limit)
        output = -pid->output_limit;

    return output;
}