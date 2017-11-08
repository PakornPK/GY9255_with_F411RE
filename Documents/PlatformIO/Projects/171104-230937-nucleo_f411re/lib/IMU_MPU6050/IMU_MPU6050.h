#include <mbed.h>

#ifndef IMU_MPU6050_H

/******************************************************/
#define IMU_MPU6050_H

#define PI 3.14159265358979323846

#define GYRO_CONFIG 0x1B
#define INT_PIN_CFG 0x37
#define INT_ENABLE 0x38
#define POWER_MGMT_1 0x6B
#define ADDRESS 0x68 << 1

class IMU_MPU6050 {

public:

  IMU_MPU6050 (PinName sda, PinName scl);
  //~IMU_MPU6050();

  void gyro_calid();
  float getYaw();

private:
  Timer _Time_Yaw;

  char _yaw_cmd = 0x47;

  float _gyro_avr;
  float _yaw;

  I2C _i2c;

  void write_setting_byte(char sub_adr, char data);
  void read_byte(char* cmd, char* data);
  void read_word(char* cmd, int* data);

  void setting_gyro_scale();
  void initMPU();

};

/******************************************************/

#endif
