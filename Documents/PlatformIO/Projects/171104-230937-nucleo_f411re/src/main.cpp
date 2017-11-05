#include "main.h"

int main() {


    // put your setup code here, to run once:
    pc.printf("Hello world\n");
    float z,yaw;
    int gyro_z;

    write_setting_byte(power_mgmt_1,0);

    init_offset();
    //pc.printf("%d",gyro_avr);

    while(1) {
        t.start();
        read_word(&roll_cmd, &gyro_z);
        gyro_z = gyro_z - gyro_avr ;
        z = ((gyro_z / 32767.0) * 250.0) * (PI/180);
        t.stop();
        yaw += -t.read() * z;
        pc.printf(" Yaw: %f time : %f\n",z,t.read());
        t.reset();
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
  char high;
  char low;

  read_byte(cmd,&high);
  (*cmd++);
  read_byte(cmd,&low);
  (*cmd--);

  *data = (int16_t)((high << 8) | low);
}

void read_word_2c(char*cmd, float*data){
  int val;
  read_word(cmd,&val);

  if (val >= 0x8000){
    *data = -((65535 -val) + 1);
  } else {
    *data = val;
  }
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
