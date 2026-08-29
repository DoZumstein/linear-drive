#ifndef COMMS_H_
#define COMMS_H_

#include <stdint.h>
#include <stdbool.h>
#include "motor.h"

#define CMD_BUFFER_SIZE 128

typedef enum
{
    CMD_UNKNOWN,
    CMD_MOTOR_STOP,
    CMD_MOTOR_OFF,
    CMD_MOTOR_CW,
    CMD_MOTOR_CCW,
    CMD_GET_SPEED,
    CMD_SET_SPEED,
} UartCommand;

typedef struct
{
    char input_buffer[CMD_BUFFER_SIZE - 1];
    uint8_t idx;
    bool input_ready;
    char cmd_str[CMD_BUFFER_SIZE - 1];
    char arg_str[CMD_BUFFER_SIZE - 1];
    UartCommand command;
    bool command_parsed;
} UartCommandHandle;

extern UartCommandHandle uart_handle;

typedef struct UartCommandMap_
{
    const char *name;
    UartCommand command;
} UartCommandMap;

void uart_start_rx();
void parse_msg(UartCommandHandle *uart_handle, Motor *motor);

#endif