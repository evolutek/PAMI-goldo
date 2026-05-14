#include <Servo.h>
#include "actuators.h"
#include "common.h"

/* Servos */
#define SERVO1 17
#define SERVO2 18
#define SERVO3 40
#define SERVO4 42
#define SERVO_L SERVO4
#define SERVO_R SERVO1

/* Buzzer */
#define BUZZER 14

Servo servos = Servo();
Pwm pwm = Pwm();

void init_servos() {
    servos.attach(SERVO1);
    servos.attach(SERVO2);
    servos.attach(SERVO3);
    servos.attach(SERVO4);
}

void servo_mid() {
    servos.write(SERVO_L, 90);
    servos.write(SERVO_R, 90);
}

void servo_lift() {
    servos.write(SERVO4, 157); //reduced from 160 (too much for PAMI2)
    servos.write(SERVO1, 13); //reduced from 10 (too much for PAMI2)
}

void servo_drop() {
    servos.write(SERVO_L, 60);
    servos.write(SERVO_R, 110);
}

void servo_drop_l() {
    if (current_side == SIDE_YELLOW)
        servos.write(SERVO_L, 60);
    else
        servos.write(SERVO_R, 60);
}

void servo_drop_r() {
    if (current_side == SIDE_YELLOW)
        servos.write(SERVO_R, 110);
    else
        servos.write(SERVO_L, 110);
}