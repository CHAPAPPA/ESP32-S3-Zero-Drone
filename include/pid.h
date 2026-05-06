#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float kp;
    float ki;
    float kd;

    float integral;
    float prev_error;

    float integral_limit;
    float output_limit;
} PID_t;

/**
 * @brief Initialize PID controller
 */
void PID_Init(PID_t *pid, float kp, float ki, float kd,
              float integral_limit, float output_limit);

/**
 * @brief Reset PID internal state
 */
void PID_Reset(PID_t *pid);

/**
 * @brief Update PID controller
 * 
 * @param pid Pointer to PID struct
 * @param setpoint Desired value
 * @param measurement Current value
 * @param dt Time delta in seconds
 * @return float Control output
 */
float PID_Update(PID_t *pid, float setpoint, float measurement, float dt);

#ifdef __cplusplus
}
#endif

#endif // PID_H