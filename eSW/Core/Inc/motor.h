#ifndef MOTOR_H_
#define MOTOR_H_
#include <stdint.h>

#define MOTOR_DUTY_START_VALUE 10
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
    uint16_t last_encoder_count;
} Motor;

void motor_control(Motor *motor);
int16_t get_rpm(Motor *motor);
void set_motor_speed(Motor *motor, uint16_t speed);
#endif