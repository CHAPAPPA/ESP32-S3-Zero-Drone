#ifndef MPU_H
#define MPU_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#ifdef __cplusplus
extern "C" {
#endif

// Struct to store sensor data (cleaner for drone use)
typedef struct {
    float ax, ay, az;
    float gx, gy, gz;
    float temperature;
} MPU_Data_t;

// Init MPU
bool MPU_Init(void);

// Read all sensor data
void MPU_Read(MPU_Data_t *data);

#ifdef __cplusplus
}
#endif

#endif