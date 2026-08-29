#ifndef MOTOR_H_
#define MOTOR_H_
#include <stdint.h>

#define MOTOR_DUTY_START_VALUE 20
#define MOTOR_DUTY_INCREMENT 10

typedef enum
{
    OFF,
    STOP,
    CLOCKWISE,
    COUNTERCLOCKWISE,
} MotorState;

typedef struct Motor_
{
    MotorState state;
    uint8_t duty_cycle;
} Motor;

void motor_control(Motor *motor);

#endif