#include <Wire.h>

const int MPU_ADDR = 0x68;

// CHANGE THESE PINS if needed
#define SDA_PIN 8
#define SCL_PIN 9

int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t gyro_x, gyro_y, gyro_z;
int16_t temperature;

char tmp_str[7];

char* convert_int16_to_str(int16_t i) {
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // IMPORTANT: ESP32-S3 must define pins
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  Serial.println("Initializing MPU6050...");

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);   // power management register
  Wire.write(0);      // wake up sensor
  if (Wire.endTransmission(true) != 0) {
    Serial.println("MPU6050 not found!");
  } else {
    Serial.println("MPU6050 connected!");
  }
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  if (Wire.endTransmission(false) != 0) {
    Serial.println("I2C write error");
    delay(500);
    return;
  }

  Wire.requestFrom(MPU_ADDR, 14, true);

  if (Wire.available() < 14) {
    Serial.println("No data from MPU6050");
    delay(500);
    return;
  }

  accelerometer_x = Wire.read() << 8 | Wire.read();
  accelerometer_y = Wire.read() << 8 | Wire.read();
  accelerometer_z = Wire.read() << 8 | Wire.read();
  temperature     = Wire.read() << 8 | Wire.read();
  gyro_x          = Wire.read() << 8 | Wire.read();
  gyro_y          = Wire.read() << 8 | Wire.read();
  gyro_z          = Wire.read() << 8 | Wire.read();

  Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));

  Serial.print(" | tmp = ");
  Serial.print(temperature / 340.00 + 36.53);

  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
  Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
  Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));

  Serial.println();

  delay(500);
}