#include "mpu.h"
#include "pid.h"

MPU_Data_t mpu_data;

PID_t pid_roll;
PID_t pid_pitch;

// Simple angle estimation (complementary filter)
float roll = 0.0f;
float pitch = 0.0f;

float alpha = 0.98f; // filter coefficient

unsigned long last_time = 0;

void setup()
{
    Serial.begin(115200);
    while (!Serial) delay(10);

    // Init MPU
    if (!MPU_Init()) {
        Serial.println("MPU init failed!");
        while (1);
    }

    // Init PID
    PID_Init(&pid_roll, 1.5f, 0.0f, 0.05f, 100.0f, 200.0f);
    PID_Init(&pid_pitch, 1.5f, 0.0f, 0.05f, 100.0f, 200.0f);

    last_time = micros();
}

void loop()
{
    // ---- Time step (dt) ----
    unsigned long now = micros();
    float dt = (now - last_time) / 1000000.0f; // seconds
    last_time = now;

    // Safety clamp (avoid crazy dt spikes)
    if (dt <= 0 || dt > 0.02f) return;

    // ---- Read MPU ----
    MPU_Read(&mpu_data);

    // ---- Compute angles ----
    // Accelerometer angles
    float acc_roll  = atan2(mpu_data.ay, mpu_data.az) * 180.0f / PI;
    float acc_pitch = atan2(-mpu_data.ax, 
                      sqrt(mpu_data.ay * mpu_data.ay + mpu_data.az * mpu_data.az)) 
                      * 180.0f / PI;

    // Gyro rates (convert rad/s → deg/s)
    float gyro_roll_rate  = mpu_data.gx * 180.0f / PI;
    float gyro_pitch_rate = mpu_data.gy * 180.0f / PI;

    // Complementary filter
    roll  = alpha * (roll  + gyro_roll_rate * dt)  + (1 - alpha) * acc_roll;
    pitch = alpha * (pitch + gyro_pitch_rate * dt) + (1 - alpha) * acc_pitch;

    // ---- PID ----
    float roll_output  = PID_Update(&pid_roll,  0.0f, roll, dt);
    float pitch_output = PID_Update(&pid_pitch, 0.0f, pitch, dt);

    // ---- Debug output ----
    Serial.print("Roll: ");
    Serial.print(roll);
    Serial.print(" | Pitch: ");
    Serial.print(pitch);

    Serial.print(" | PID R: ");
    Serial.print(roll_output);
    Serial.print(" | PID P: ");
    Serial.println(pitch_output);

    // ---- Loop timing (target ~500 Hz) ----
    delayMicroseconds(2000);
}