#include <mbed.h>

#define PI 3.14159265358979323846

I2C i2c(PB_9, PB_8);
Serial pc(USBTX, USBRX);

DigitalIn btn(USER_BUTTON);

Timer t;

//GLOBAL VARIABLE
float gyro_avr;
char roll_cmd = 0x47;


const int power_mgmt_1 = 0x6B;
const int address = 0x68 << 1;//Shift one bit because Mbed API use 8 bit

void write_setting_byte(char sub_adr, char data);
void read_byte(char* cmd, char* data);
void read_word(char* cmd,int* data);
void read_word_2c(char*cmd, float*data);

void init_offset();
void yaw_calculator();
