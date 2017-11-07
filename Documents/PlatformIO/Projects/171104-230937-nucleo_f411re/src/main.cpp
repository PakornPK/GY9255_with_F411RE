#include "main.h"

int main() {

    // put your setup code here, to run once:
    pc.printf("Hello world\n");
    led1 = 1;

    write_setting_byte(power_mgmt_1,0);
    setting_gyro_scale();

    init_offset();
    t.start();
    //pc.attach(&yaw_calculator, Serial::RxIrq);
    //process_yaw.attach(&yaw_calculator, 0.02);

    while(1) {
        if(t.read_ms() > 100){
          yaw_calculator();
          t.reset();
        }
    }
}

void write_setting_byte(char sub_adr, char data){
  char data_write[2];

  data_write[0] = sub_adr;
  data_write[1] = data;
  i2c.write(address, data_write, 2, 0);
}

void read_byte(char* cmd, char* data){
  //CMD & DATA only one byte
  i2c.write(address, cmd, 1, 1);
  i2c.read(address, data, 1, 0);
}

void read_word(char* cmd,int* data){
  while(state != false){pc.printf("OMG BUG !\n");}
  state = true;

  char high;
  char low;

  read_byte(cmd,&high);
  (*cmd++);
  read_byte(cmd,&low);
  (*cmd--);

  *data = (int16_t)((high << 8) | low);
  //pc.printf("Debug : %d \n",*data); //for debug why I2C cann't work in timerinterrupt
  state = false;
}

void setting_gyro_scale(){
  char c,cmd ;
  cmd = GYRO_CONFIG;
  read_byte(&cmd, &c);
  write_setting_byte(GYRO_CONFIG, c & ~0xE0); // Clear self-test bits [7:5]
  write_setting_byte(GYRO_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
  write_setting_byte(GYRO_CONFIG, c | 0 << 3); // Set full scale range for the gyro

  write_setting_byte(INT_PIN_CFG, 0x22);
  write_setting_byte(INT_ENABLE, 0x01);
}

void init_offset(){
  float sum_gyro =0;
  int data_now;
  for(int i = 0; i<500;i++){
    pc.printf("%d\n",i);
    read_word(&roll_cmd, &data_now);
    sum_gyro = sum_gyro +data_now;
  }

  gyro_avr = sum_gyro/500.0;
}

void yaw_calculator(){
  t_yaw.start();
  read_word(&roll_cmd, &gyro_z);
  debug_z = gyro_z;
  gyro_z = gyro_z - gyro_avr ;
  z = ((gyro_z / 32767.0) * 250.0) * (180/PI);
  t_yaw.stop();
  yaw += -t_yaw.read() * z;
  //yaw += -0.02*z;

  if (yaw < 0){
    yaw += 360;
  } else if(yaw > 360){
    yaw -= 360;
  }

  pc.printf(" Yaw: %f\n",yaw);
  //led1 = ~led1;
  t_yaw.reset();
}
