#include <mbed.h>
#include <math.h>

#define PI 3.14159265

I2C i2c(PB_9, PB_8);
Serial pc(USBTX, USBRX);

DigitalIn btn(USER_BUTTON);

const int power_mgmt_1 = 0x6B ;
const int power_mgmt_2 = 0x6C ;
const int address = 0x68 << 1;//Shift one bit because Mbed API use 8 bit

void write_setting_byte(char sub_adr, char data);
void read_byte(char* cmd, char* data);
void read_word(char* cmd,int* data);
void read_word_2c(char*cmd, int*data);

float dist(float a, float b);
float get_pitch_rotation(float x, float y, float z);
float get_roll_rotation(float x, float y, float z);
float get_yaw_rotation(float x, float y, float z);
