#ifndef MPU6050_H
#define MPU6050_H
#include "mbed.h"

#define IMU6050_GYRO_CONFIG_REG    0x1B
#define IMU6050_GYRO_RANGE_250     0
#define RA_WHO_AM_I         0x75
#define RA_CONFIG           0x1A
#define RA_USER_CTRL        0x6A
#define RA_PWR_ADDR   	    0x6B
#define RA_SMPLRT_DIV	    0x19
#define RA_GYRO_CONFIG      0x1B
#define RA_ACCEL_CONFIG     0x1C
#define RA_INT_PIN_CFG      0x37
#define RA_ACCEL_XOUT_H     0x3B
#define RA_ACCEL_XOUT_L     0x3C
#define RA_ACCEL_YOUT_H     0x3D
#define RA_ACCEL_YOUT_L     0x3E
#define RA_ACCEL_ZOUT_H     0x3F
#define RA_ACCEL_ZOUT_L     0x40
#define RA_TEMP_OUT_H       0x41
#define RA_TEMP_OUT_L       0x42
#define RA_GYRO_XOUT_H      0x43
#define RA_GYRO_XOUT_L      0x44
#define RA_GYRO_YOUT_H      0x45
#define RA_GYRO_YOUT_L      0x46
#define RA_GYRO_ZOUT_H      0x47
#define RA_GYRO_ZOUT_L      0x48
#define AF_SEL 							0x00
#define ACC_HPF 						0x00
#define FS_SEL 							0x00
#define RA_PWR_MGMT_1  			0x00
#define USER_CTRL        		0x00
#define ACC_SELF_TEST_X 		0x00
#define ACC_SELF_TEST_Y		 	0x00
#define ACC_SELF_TEST_Z 		0x00
#define G_SELF_TEST_X 			0x00
#define G_SELF_TEST_Y 			0x00
#define G_SELF_TEST_Z 			0x00
#define PIN_CFG							2
#define CONFIG							4
#define SMPLRT_DIV					19



class MPU6050Class{

  public:
    static const int MPU6050 = 0x68<<1;
    MPU6050Class(PinName sda, PinName scl);
    MPU6050Class(I2C &i2c) : i2c(i2c) {
        InitMPU();
    }
    ~MPU6050Class();
    void InitMPU();
    char reg_value[2];
    void WriteData(uint8_t address, uint8_t subAddress, uint8_t data);
    int ReadData(int adress_L, int adress_H);
    char read(char address);
    void write(char address, char data);
    void setGyroRange( char range );

  private:
      I2C &i2c;

      char i2cRaw[sizeof(I2C)];

};
extern MPU6050Class MPU6050;
#endif
