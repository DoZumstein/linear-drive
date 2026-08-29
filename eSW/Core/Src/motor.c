#include "motor.h"
#include "main.h"
#include "stm32wbxx_hal.h"
#include "tim.h"
#include "lptim.h"

#define DELTA_TIME_S 0.005f
#define COUNTS_PER_REVELUTION 2797
#define MAX_RPM 251

static void set_pwm(uint8_t duty)
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim2);
    uint32_t compare = (uint32_t)(((uint64_t)(arr * duty)) / 100);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, compare);
}

void set_motor_speed(Motor *motor, uint16_t speed)
{
    motor->duty_cycle = (uint8_t)((float)speed * 100.f / MAX_RPM);
}

void motor_control(Motor *motor)
{
    switch (motor->state)
    {
    case STOP:
    {
        // Motor
        HAL_GPIO_WritePin(DC_EN_GPIO_Port, DC_EN_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(DC_IN2_GPIO_Port, DC_IN2_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);

        // LED
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
        break;
    }
    case CLOCKWISE:
    {
        // Motor
        HAL_GPIO_WritePin(DC_EN_GPIO_Port, DC_EN_Pin, GPIO_PIN_SET);
        uint8_t duty = 100 - motor->duty_cycle;
        set_pwm(duty);
        HAL_GPIO_WritePin(DC_IN2_GPIO_Port, DC_IN2_Pin, GPIO_PIN_SET);

        // LED
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);

        break;
    }
    case COUNTERCLOCKWISE:
    {
        // Motor
        HAL_GPIO_WritePin(DC_EN_GPIO_Port, DC_EN_Pin, GPIO_PIN_SET);
        set_pwm(motor->duty_cycle);
        HAL_GPIO_WritePin(DC_IN2_GPIO_Port, DC_IN2_Pin, GPIO_PIN_RESET);

        // LED
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
        break;
    }
    case OFF:
    {
        // Motor
        HAL_GPIO_WritePin(DC_EN_GPIO_Port, DC_EN_Pin, GPIO_PIN_RESET);

        // Led
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
        break;
    }

    default:
        break;
    }
}

int16_t get_rpm(Motor *motor)
{
    uint32_t encoder_count = HAL_LPTIM_ReadCounter(&hlptim1);
    int16_t delta_count = (int16_t)(encoder_count - motor->last_encoder_count);
    float counts_per_sec = (float)delta_count / DELTA_TIME_S;
    float rev_per_min = (counts_per_sec / COUNTS_PER_REVELUTION) * 60;
    motor->last_encoder_count = encoder_count;
    return rev_per_min;
}