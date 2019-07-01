#ifndef PICTL_H
#define PICTL_H

#define IN0     21
#define IN1     22
#define IN2     23
#define IN3     24

#define MAX_PWM 1024

int pi_init();

int pi_run(float speed, float turn);

#endif // !PICTL_H


