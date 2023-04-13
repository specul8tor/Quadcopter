// PID Loop
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <cstdlib>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include "PI.h"
#include <string>
#include <math.h>
using namespace std;


void PI_initialize(){
integrate = 0;
previous_error = 0;
differentiate = 0;
previous_measure = 0;
proportional = 0;
output = 0;   
}

void IMU_filter(){

// gyro integration
pitch += gx*delta_t;
roll += gy*delta_t;
yaw += gz*delta_t;

// acc LPF
Ax = 0.9*Ax + 0.1*(Ax_raw - Ax)/2;
Ay = 0.9*Ay + 0.1*(Ay_raw - Ay)/2;
Az = 0.9*Az + 0.1*(Az_raw - Az)/2;

// check accelerometer is not experiencing external acceleration
float A_mag = fabsf(Ax) + fabsf(Ay) + fabsf(Az); // close to 1g
if(fabsf(A_mag - 1) < 0.3){
    // apply trig equations to convert accelerometer data
    pitch = (0.99*pitch) + (0.01*( (180/M_PI)*(atan(Ax/(sqrt(Ay^2 + Az^2)))) ));
    roll = (0.99*roll) + (0.01*( (180/M_PI)*(atan2(Ay/Az))));
}

// end of fuse
}





uint16_t PI(){
    // measured = sensor inputs
        error = abs(measure - setpoint);
        proportional = Kp * error;
        integrate = (integrate + (error)/1000) * Ki;
        //cout<<"error, proportional, integrate: " << error << " " <<  proportional <<  " " << integrate << "\n " << endl;

        // dynamic integrator clamping

        if (MaxLim > proportional){
            MaxLimIntegrate = MaxLim - proportional;
        }
        else{
            MaxLimIntegrate = 0;
        }


        if (MinLim < proportional){
            MinLimIntegrate = MinLim - proportional;
        }
        else{
            MinLimIntegrate = 0;
        }


        output = proportional + integrate;
        //cout << "pre-output " << output << endl; 

        // apply output limits 

        if (output > MaxLim){
            output = MaxLim;
        }
        else if (output < MinLim)
        {
            output = MinLim;
        }

        previous_error = error;
        previous_measure = measure;
        //cout << "post-output " << output << endl;
        return output;
}


int main()
{
    // srand(time(NULL));
    // uint16_t random = rand() % 50 + 1; // for test apply this function and imitate random values for the Gyroscope
    // measure = random;
    setpoint = 25; // should be changed to the IMU processed angles. 
    // PI loop will be used three times for each angle. roll pitch and yaw
    PI_initialize();
    // uint16_t a = PI();
    // cout<<a<<endl;

    for(int i = 0; i < 25; i++)
    {
        uint16_t a = PI();
        cout<< a <<endl;
    }

    // integrate 

}
