#include "comms.h"
#include "stm32wbxx_hal.h"
#include "usart.h"
#include <string.h>
#include <stdlib.h>

static char rx_buffer[1];
UartCommandHandle uart_handle = {0};

static const UartCommandMap command_table[] = {
    {"stop", CMD_MOTOR_STOP},
    {"off", CMD_MOTOR_OFF},
    {"cw", CMD_MOTOR_CW},
    {"ccw", CMD_MOTOR_CCW},
    {"getspeed", CMD_GET_SPEED},
    {"setspeed", CMD_SET_SPEED},
};

void uart_start_rx()
{
    HAL_UART_Receive_IT(&huart1, (uint8_t *)rx_buffer, sizeof(rx_buffer));
}

static UartCommand get_command(const char *str)
{
    for (uint8_t i = 0; i < sizeof(command_table) / sizeof(command_table[0]); i++)
    {
        if (strcmp(str, command_table[i].name) == 0)
        {
            return command_table[i].command;
        }
    }

    return CMD_UNKNOWN;
}

static void split_command(char *input, char *cmd, char *arg)
{
    char *space = strchr(input, ' ');

    if (space == NULL)
    {
        strcpy(cmd, input);
        arg[0] = '\0';
    }

    *space = '\0';
    strcpy(cmd, input);
    strcpy(arg, space + 1);
}

void parse_msg(UartCommandHandle *uart_handle, Motor *motor)
{
    if (!uart_handle->input_ready || uart_handle->command_parsed)
    {
        return;
    }

    split_command(uart_handle->input_buffer, uart_handle->cmd_str, uart_handle->arg_str);
    UartCommand command = get_command(uart_handle->cmd_str);

    switch (command)
    {
    case CMD_UNKNOWN:
        // Error msg
        break;
    case CMD_MOTOR_STOP:
        motor->state = STOP;
        break;
    case CMD_MOTOR_OFF:
        motor->state = OFF;
        break;
    case CMD_MOTOR_CW:
        motor->state = CLOCKWISE;
        break;
    case CMD_MOTOR_CCW:
        motor->state = COUNTERCLOCKWISE;
        break;
    case CMD_GET_SPEED:
        // transmit speed
        break;
    case CMD_SET_SPEED:
        uint16_t speed = (uint16_t)atoi(uart_handle->arg_str);
        set_motor_speed(motor, speed);
        break;
    default:
        break;
    }
    uart_handle->command_parsed = true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart != &huart1)
    {
        return;
    }

    if (*rx_buffer == '\n' || uart_handle.idx >= (CMD_BUFFER_SIZE - 1))
    {
        uart_handle.input_buffer[uart_handle.idx] = '\0';
        uart_handle.input_ready = true;
        uart_handle.command_parsed = false;
        uart_handle.idx = 0;
    }
    else
    {
        uart_handle.input_buffer[uart_handle.idx++] = *rx_buffer;
    }

    HAL_UART_Receive_IT(&huart1, (uint8_t *)rx_buffer, sizeof(rx_buffer));
}