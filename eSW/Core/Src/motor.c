#include "motor.h"
#include "main.h"
#include "stm32wbxx_hal.h"
#include "tim.h"

void motor_control(Motor *motor)
{
    switch (motor->state)
    {
    case STOP:
    {
        // Motor
        HAL_GPIO_WritePin(DC_IN2_GPIO_Port, DC_IN2_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
        motor->duty_cycle = MOTOR_DUTY_START_VALUE;
        // LED
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        break;
    }
    case CLOCKWISE:
    {
        // Motor
        uint8_t duty = 100 - motor->duty_cycle;
        uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim2);
        uint32_t compare = (uint32_t)(((uint64_t)(arr * duty)) / 100);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, compare);
        HAL_GPIO_WritePin(DC_IN2_GPIO_Port, DC_IN2_Pin, GPIO_PIN_SET);

        // LED
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
        if (motor->duty_cycle >= 100)
        {
            motor->duty_cycle = 100;
            HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
        }
        break;
    }
    case COUNTERCLOCKWISE:
    {
        // Motor
        uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim2);
        uint32_t compare = (uint32_t)(((uint64_t)(arr * motor->duty_cycle)) / 100);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, compare);
        HAL_GPIO_WritePin(DC_IN2_GPIO_Port, DC_IN2_Pin, GPIO_PIN_RESET);

        // LED
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
        if (motor->duty_cycle >= 100)
        {
            motor->duty_cycle = 100;
            HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
        }
        break;
    }
    case OFF:
    {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DC_EN_GPIO_Port, DC_EN_Pin, GPIO_PIN_RESET);
        break;
    }

    default:
        break;
    }
}