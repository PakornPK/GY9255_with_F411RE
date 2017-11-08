#include <mbed.h>
#include <IMU_MPU6050.h>

IMU_MPU6050 GY_IMU(PB_9, PB_8);
Serial pc(USBTX, USBRX);

Timer t;

float ans_;

int main(){
  GY_IMU.gyro_calid();
  t.start();

  while(1){
    if(t.read_ms() > 100){
      ans_ = GY_IMU.getYaw();
      pc.printf(" Yaw: %f\n",ans_);

      t.reset();
    }
  }
}
