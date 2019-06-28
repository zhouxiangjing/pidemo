#ifndef PICTL_H
#define PICTL_H

#define IN0     0
#define IN1     1
#define IN2     2
#define IN3     3

#define MAX_PWM 100

int pi_init();

int pi_run(float speed, float turn);

#endif // !PICTL_H


