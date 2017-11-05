#include "main.h"

int main() {

    // put your setup code here, to run once:
    char roll_cmd;

    float x,y,z;
    int gyro_x,gyro_y,gyro_z,accel_x,accel_y,accel_z;

    write_setting_byte(power_mgmt_1,0);

    while(1) {
        // put your main code here, to run repeatedly:
        //while(btn == 1);

        //pc.printf("-----------Gyro------------\n");

        //get_gyro(&gyro_x,&gyro_y,&gyro_z);

        roll_cmd = 0x43;
        read_word_2c(&roll_cmd, &gyro_x);
        roll_cmd = 0x45;
        read_word_2c(&roll_cmd, &gyro_y);
        roll_cmd = 0x47;
        read_word_2c(&roll_cmd, &gyro_z);

        //pc.printf("Gyro X: %f \n",(float)gyro_x/131);
        //pc.printf("Gyro Y: %f \n",(float)gyro_y/131);
        //pc.printf("Gyro Z: %f \n",(float)gyro_z/131);

        //pc.printf("-----------Accelo------------\n");

        //get_accelometer(&accel_x,&accel_y,&accel_z);

        roll_cmd = 0x3B;
        read_word_2c(&roll_cmd, &accel_x);
        roll_cmd = 0x3D;
        read_word_2c(&roll_cmd, &accel_y);
        roll_cmd = 0x3F;
        read_word_2c(&roll_cmd, &accel_z);

        //pc.printf("Accelo X: %f \n",(float)accel_x/16384.0);
        //pc.printf("Accelo Y: %f \n",(float)accel_y/16384.0);
        //pc.printf("Accelo Z: %f \n",(float)accel_z/16384.0);

        x = get_pitch_rotation((float)accel_x/16384.0, (float)accel_y/16384.0, (float)accel_z/16384.0);
        y = get_roll_rotation((float)accel_x/16384.0, (float)accel_y/16384.0, (float)accel_z/16384.0);
        z = get_yaw_rotation((float)accel_x/16384.0, (float)accel_y/16384.0, (float)accel_z/16384.0);

        pc.printf("X: %f, Y: %f, Z: %f \n",x,y,z);

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
  *data = (high << 8) + low;
}

void read_word_2c(char*cmd, int*data){
  int val;
  read_word(cmd,&val);

  if (val >= 0x8000){
    *data = -((65535 -val) + 1);
  } else {
    *data = val;
  }
}

void get_gyro(int* x, int* y, int* z){
  char roll_cmd;

  roll_cmd = 0x43;
  read_word_2c(&roll_cmd, x);
  *x = *x / 131;
  roll_cmd = 0x45;
  read_word_2c(&roll_cmd, y);
  *y = *y / 131;
  roll_cmd = 0x47;
  read_word_2c(&roll_cmd, z);
  *z = *z / 131;
}

void get_accelometer(int* x, int* y, int* z){
  char roll_cmd;

  roll_cmd = 0x3B;
  read_word_2c(&roll_cmd, x);
  *x = *x / 16384.0;
  roll_cmd = 0x3D;
  read_word_2c(&roll_cmd, y);
  *y = *y / 16384.0;
  roll_cmd = 0x3F;
  read_word_2c(&roll_cmd, z);
  *z = *z /16384.0;
}

float dist(float a, float b){
  return sqrt(a*a+b*b);
}

float get_pitch_rotation(float x, float y ,float z){
  return -(atan2(x,dist(y,z)) * 180/PI);
}

float get_roll_rotation(float x, float y, float z){
  return atan2(y, dist(x,z)) * 180/PI;
}

float get_yaw_rotation(float x, float y, float z){
  return atan2(dist(y,z),z) * 180/PI;
}
