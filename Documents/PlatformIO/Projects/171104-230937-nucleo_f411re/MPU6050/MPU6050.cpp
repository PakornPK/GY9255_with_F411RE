#include "MPU6050.h"
#include "mbed.h"
#include <new>
MPU6050Class::MPU6050Class(PinName sda, PinName scl) : i2c(*reinterpret_cast<I2C*>(i2cRaw))
{
  new(i2cRaw) I2C(I2C_SDA, I2C_SCL);
  InitMPU();
}

MPU6050Class::~MPU6050Class()
{
  if(&i2c == reinterpret_cast<I2C*>(&i2cRaw))
      reinterpret_cast<I2C*>(&i2cRaw)->~I2C();
}

void  MPU6050Class::InitMPU(){
  i2c.frequency(400000);
	WriteData(MPU6050,RA_PWR_ADDR,RA_PWR_MGMT_1);
	wait(0.1);
	WriteData(MPU6050,RA_USER_CTRL,USER_CTRL);
	WriteData(MPU6050, RA_INT_PIN_CFG,PIN_CFG);
	WriteData(MPU6050,RA_SMPLRT_DIV,SMPLRT_DIV);
	int GConfig = G_SELF_TEST_X | G_SELF_TEST_Y | G_SELF_TEST_Z | FS_SEL;
	int AConfig = ACC_SELF_TEST_X | ACC_SELF_TEST_Y | ACC_SELF_TEST_Z | AF_SEL | ACC_HPF;
	WriteData(MPU6050,RA_GYRO_CONFIG,GConfig);
	WriteData(MPU6050,RA_ACCEL_CONFIG,AConfig);

}

int MPU6050Class::ReadData(int adress_L, int adress_H){
  char data_L;
	char data_H;
	char adress_data_L[1] ;
	char adress_data_H[1] ;
	adress_data_L[0]= adress_L;
	adress_data_H[0]= adress_H;
	i2c.write(MPU6050, adress_data_L, 1,1);
	i2c.read(MPU6050,&data_L, 1,0);
	i2c.write(MPU6050, adress_data_H, 1,1);
	i2c.read(MPU6050,&data_H, 1,0);
	return (int16_t)((data_H<<8) | data_L);

}
 void MPU6050Class::WriteData(uint8_t address, uint8_t subAddress, uint8_t data)
{
   char data_write[2];
   data_write[0] = subAddress;
   data_write[1] = data;
   i2c.write(address, data_write, 2, false);
}

char MPU6050Class::read(char address) {
    char retval;
    i2c.write(MPU6050 * 2, &address, 1, true);
    i2c.read(MPU6050 * 2, &retval, 1);
    return retval;
}

void MPU6050Class::write(char address, char data) {
    char temp[2];
    temp[0]=address;
    temp[1]=data;
    i2c.write(MPU6050 * 2,temp,2);
}

void MPU6050Class::setGyroRange( char range ) {
    char temp;
    char currentGyroRange = range;
    range = range & 0x03;
    temp = read(IMU6050_GYRO_CONFIG_REG);
    temp &= ~(3<<3);
    temp = temp + range<<3;
    write(IMU6050_GYRO_CONFIG_REG, temp);
}
