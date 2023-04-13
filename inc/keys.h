
#include <XPD.h>
#include "utils.h"

/*
motor[0] is motor 1 on pcb
motor[1] is motor 2 on pcb
motor[2] is motor 3 on pcb
motor[3] is motor 4 on pcb
*/

#define W 119
#define A 97
#define S 115
#define D 100
#define I 105
#define J 106
#define K 107
#define L 108
#define R 114

#define DELAY 1000
#define STEP 100

static void up(){
    // if (motors[0].throttle == 48){
    //     motors[0].throttle += 199;
    //     motors[1].throttle += 199;
    //     motors[2].throttle += 199;
    //     motors[3].throttle += 199;
    // }
    // else if(!(motors[0].throttle == 1847)){
        motors[0].throttle += STEP;
        motors[1].throttle += STEP;
        motors[2].throttle += STEP;
        motors[3].throttle += STEP;
    // }
}

static void down(){
    // if(motors[0].throttle != 48){
        motors[0].throttle -= STEP;
        motors[1].throttle -= STEP;
        motors[2].throttle -= STEP;
        motors[3].throttle -= STEP;
    // }
}

static void right(){
    motors[0].throttle += STEP;
    motors[1].throttle += STEP;
    motors[2].throttle -= STEP;
    motors[3].throttle -= STEP;
    wait_ms(DELAY);
    motors[0].throttle -= STEP;
    motors[1].throttle -= STEP;
    motors[2].throttle += STEP;
    motors[3].throttle += STEP;
}

static void left(){
    motors[2].throttle += STEP;
    motors[3].throttle += STEP;
    motors[0].throttle -= STEP;
    motors[1].throttle -= STEP;
    wait_ms(DELAY);
    motors[2].throttle -= STEP;
    motors[3].throttle -= STEP;
    motors[0].throttle += STEP;
    motors[1].throttle += STEP;
}

static void forward(){
    motors[0].throttle += STEP;
    motors[2].throttle += STEP;
    motors[1].throttle -= STEP;
    motors[3].throttle -= STEP;
    wait_ms(DELAY);
    motors[0].throttle -= STEP;
    motors[2].throttle -= STEP;
    motors[1].throttle += STEP;
    motors[3].throttle += STEP;
}

static void backward(){
    motors[1].throttle += STEP;
    motors[3].throttle += STEP;
    motors[0].throttle -= STEP;
    motors[2].throttle -= STEP;
    wait_ms(DELAY);
    motors[1].throttle -= STEP;
    motors[3].throttle -= STEP;
    motors[0].throttle += STEP;
    motors[2].throttle += STEP;
}

static void rotate_right(){
    motors[0].throttle += STEP;
    motors[3].throttle += STEP;
    motors[1].throttle -= STEP;
    motors[2].throttle -= STEP;
    wait_ms(DELAY);
    motors[0].throttle -= STEP;
    motors[3].throttle -= STEP;
    motors[1].throttle += STEP;
    motors[2].throttle += STEP;
}

static void rotate_left(){
    motors[0].throttle -= STEP;
    motors[3].throttle -= STEP;
    motors[1].throttle += STEP;
    motors[2].throttle += STEP;
    wait_ms(DELAY);
    motors[0].throttle += STEP;
    motors[3].throttle += STEP;
    motors[1].throttle -= STEP;
    motors[2].throttle -= STEP;
}

static void stop(){
    motors[0].throttle = 0;
    motors[2].throttle = 0;
    motors[1].throttle = 0;
    motors[3].throttle = 0;
}

static void key_cmd(){
    uint16_t cmd;
    while(1) {
        cmd = xpd_getchar_timeout(1);
        if (cmd != 0xFFFF && cmd != 10 && (cmd == W || cmd == A || cmd == S || cmd == D || \
                                           cmd == I || cmd == J || cmd == K || cmd == L || cmd == R))
        {
#ifdef VERBOSE_KEY
            print_int("CMD: ", cmd, XPD_Flag_UnsignedDecimal, 1);

            print_int("Before: ", motors[0].throttle, XPD_Flag_UnsignedDecimal, 1);
            print_int("Before: ", motors[1].throttle, XPD_Flag_UnsignedDecimal, 1);
            print_int("Before: ", motors[2].throttle, XPD_Flag_UnsignedDecimal, 1);
            print_int("Before: ", motors[3].throttle, XPD_Flag_UnsignedDecimal, 1);
            xpd_puts("\n");
#endif
            switch(cmd){
                case W:
                    up();
                    break;
                case S:
                    down();
                    break;
                case A:
                    left();
                    break;
                case D:
                    right();
                    break;
                case I:
                    forward();
                    break;
                case K:
                    backward();
                    break;
                case J:
                    rotate_left();
                    break;
                case L:
                    rotate_right();
                    break;
                case R:
                    stop();
                    break;
            }
#ifdef VERBOSE_KEY
            print_int("After: ", motors[0].throttle, XPD_Flag_UnsignedDecimal, 1);
            print_int("After: ", motors[1].throttle, XPD_Flag_UnsignedDecimal, 1);
            print_int("After: ", motors[2].throttle, XPD_Flag_UnsignedDecimal, 1);
            print_int("After: ", motors[3].throttle, XPD_Flag_UnsignedDecimal, 1);
            xpd_puts("\n");
#endif
        }
    }
}