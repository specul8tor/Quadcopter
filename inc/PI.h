// PID Header File
// create a structure to store these instead
#include <stdint.h>

// variables
uint16_t integrate;
uint16_t proportional;
uint16_t previous_error;
uint16_t differentiate;
uint16_t previous_measure;
uint16_t output;
uint16_t correction;
uint16_t measure;
uint16_t error;
double Gx;
double Gy;
double Gz;
double delta_t;

double Ax;
double Ay;
double Az;

double Ax_raw;
double Ay_raw;
double Az_raw;

double theta;
double phi;
double zeta;

double pitch;
double roll;
double yaw;


// constants
uint16_t Kp = 5;
uint16_t Ki = 5;
uint16_t Kd = 2;
uint16_t tau;
float T = 0.01; //sampling time 10ms
uint16_t MaxLimIntegrate; 
uint16_t MinLimIntegrate; // limits for the integrator
uint16_t setpoint;
uint16_t MinLim = 0;
uint16_t MaxLim = 50; 