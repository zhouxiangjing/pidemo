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

    printf("pi_run start.\n");
    int IN0_val = 0;
    int IN1_val = 0;
    int IN2_val = 0;
    int IN3_val = 0;

    if(speed > 0) {
        if(turn >= 0) {
            // 正向 右拐
            printf("forward right.\n");
            IN0_val = (int)(speed*MAX_PWM);
            IN2_val = (int)(speed*MAX_PWM*(1-turn));
        } else {
            // 正向 左拐
            printf("forward left.\n");
            IN0_val = (int)(speed*MAX_PWM*(1+turn));
            IN2_val = (int)(speed*MAX_PWM);
        }
    } else if(speed < 0) {
        speed = -speed;
        if(turn >= 0) {
            // 后退 靠右
            printf("back right.\n");
            IN1_val = (int)(speed*MAX_PWM);
            IN3_val = (int)(speed*MAX_PWM*(1-turn));
        } else {
            // 后退 靠左
            printf("back left.\n");
            IN1_val = (int)(speed*MAX_PWM*(1+turn));
            IN3_val = (int)(speed*MAX_PWM);
        }
    } else {
        printf("stop.\n");
    }

    printf("INT0_val=%d INT1_val=%d INT2_val=%d INT3_val=%d.\n", INT0_val, INT1_val, INT2_val, INT3_val);
    softPwmWrite(IN0, INT0_val);
    softPwmWrite(IN1, INT1_val);
    softPwmWrite(IN2, INT2_val);
    softPwmWrite(IN3, INT3_val);

    printf("pi_run end.\n");
    return 0;
}