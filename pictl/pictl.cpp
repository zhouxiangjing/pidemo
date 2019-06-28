#include "pictl.h"

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

int pi_init() {

    if (wiringPiSetup() == -1) {
        printf("pi_init gpio setup error!\n");
        return -1;
    }

    int n1 = softPwmCreate(IN0, 0, MAX_PWM);
    int n2 = softPwmCreate(IN1, 0, MAX_PWM);
    int n3 = softPwmCreate(IN2, 0, MAX_PWM);
    int n4 = softPwmCreate(IN3, 0, MAX_PWM);

    if(0 != n1 || 0 != n2 || 0 != n3 || 0 != n4 ) {
        printf("pi_init gpio create error!\n");
        return -2;
    }

    return 0;
}

int pi_run(float speed, float turn) {

    int INT0_val = 0;
    int INT1_val = 0;
    int INT2_val = 0;
    int INT3_val = 0;

    if(speed > 0) {
        if(turn >= 0) {
            // 正向 右拐
            INT0_val = (int)(speed*MAX_PWM);
            INT2_val = (int)(speed*MAX_PWM*(1-turn));
        } else {
            // 正向 左拐
            INT0_val = (int)(speed*MAX_PWM*(1+turn));
            INT2_val = (int)(speed*MAX_PWM);
        }
    } else if(speed < 0) {
        speed = -speed;
        if(turn >= 0) {
            // 后退 靠右
            INT1_val = (int)(speed*MAX_PWM);
            INT3_val = (int)(speed*MAX_PWM*(1-turn));
        } else {
            // 后退 靠左
            INT1_val = (int)(speed*MAX_PWM*(1+turn));
            INT3_val = (int)(speed*MAX_PWM);
        }
    }

    softPwmWrite(IN0, INT0_val);
    softPwmWrite(IN1, INT1_val);
    softPwmWrite(IN2, INT2_val);
    softPwmWrite(IN3, INT3_val);

    return 0;
}