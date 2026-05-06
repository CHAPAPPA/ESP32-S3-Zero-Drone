#include "mpu.h"

static Adafruit_MPU6050 mpu;

bool MPU_Init(void)
{
    Serial.println("Initializing MPU6050...");

    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        return false;
    }

    Serial.println("MPU6050 Found!");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

    Serial.println("MPU configured");
    return true;
}

void MPU_Read(MPU_Data_t *data)
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    data->ax = a.acceleration.x;
    data->ay = a.acceleration.y;
    data->az = a.acceleration.z;

    data->gx = g.gyro.x;
    data->gy = g.gyro.y;
    data->gz = g.gyro.z;

    data->temperature = temp.temperature;
}