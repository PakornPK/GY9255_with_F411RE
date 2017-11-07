#include <mbed.h>

#define PI 3.14159265358979323846

#define GYRO_CONFIG 0x1B
#define INT_PIN_CFG 0x37
#define INT_ENABLE  0x38

I2C i2c(PB_9, PB_8);
Serial pc(USBTX, USBRX);

DigitalIn btn(USER_BUTTON);
DigitalOut led1(LED1);

Timer t,t_yaw;
Ticker process_yaw;

//GLOBAL VARIABLE
bool state = false;

float gyro_avr;
char roll_cmd = 0x47;
float z,yaw;
int gyro_z,debug_z;



const int power_mgmt_1 = 0x6B;
const int address = 0x68 << 1;//Shift one bit because Mbed API use 8 bit

void write_setting_byte(char sub_adr, char data);
void read_byte(char* cmd, char* data);
void read_word(char* cmd,int* data);

void setting_gyro_scale();
void init_offset();
void yaw_calculator();
