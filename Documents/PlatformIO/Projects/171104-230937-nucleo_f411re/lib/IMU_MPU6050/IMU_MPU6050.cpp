#include "IMU_MPU6050.h"
#include <new>

IMU_MPU6050::IMU_MPU6050(PinName sda, PinName scl) : _i2c(sda, scl)
{
  initMPU();
}

void IMU_MPU6050::initMPU()
{
  write_setting_byte(POWER_MGMT_1, 0x00);

  setting_gyro_scale();
}

void IMU_MPU6050::write_setting_byte(char sub_adr, char data)
{
  char data_write[2];

  data_write[0] = sub_adr;
  data_write[1] = data;
  _i2c.write(ADDRESS, data_write, 2, 0);
}

void IMU_MPU6050::read_byte(char *cmd, char *data)
{
  _i2c.write(ADDRESS, cmd, 1, 1);
  _i2c.read(ADDRESS, data, 1, 0);
}

void IMU_MPU6050::read_word(char *cmd, int *data)
{
  char high;
  char low;

  read_byte(cmd,&high);
  (*cmd++);
  read_byte(cmd,&low);
  (*cmd--);

  *data = (int16_t)((high << 8) | low);
  //printf("%d\n",*data);
}

void IMU_MPU6050::setting_gyro_scale()
{
  char c,cmd ;
  cmd = GYRO_CONFIG;
  read_byte(&cmd, &c);
  write_setting_byte(GYRO_CONFIG, c & ~0xE0); // Clear self-test bits [7:5]
  write_setting_byte(GYRO_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
  write_setting_byte(GYRO_CONFIG, c | 0 << 3); // Set full scale range for the gyro

  write_setting_byte(INT_PIN_CFG, 0x22);
  write_setting_byte(INT_ENABLE, 0x01);
}

void IMU_MPU6050::gyro_calid()
{
  float sum_gyro =0;
  int data_now;
  for(int i = 0; i<500;i++){
    read_word(&_yaw_cmd, &data_now);
    sum_gyro = sum_gyro +data_now;
  }

  _gyro_avr = sum_gyro/500.0;
  //printf("%f\n",_gyro_avr);
}

float IMU_MPU6050::getYaw()
{
  int _gyro_z;
  float _z_value;

  _Time_Yaw.start();

  read_word(&_yaw_cmd, &_gyro_z);
  _gyro_z = _gyro_z - _gyro_avr ;
  _z_value = ((_gyro_z / 32767.0) * 250.0) * (180/PI);
  _Time_Yaw.stop();

  _yaw += -_Time_Yaw.read() * _z_value;

  if (_yaw < 0){
    _yaw += 360;
  } else if(_yaw > 360){
    _yaw -= 360;
  }

  //printf("%f\n",_yaw);

  _Time_Yaw.reset();
  return _yaw;
}
