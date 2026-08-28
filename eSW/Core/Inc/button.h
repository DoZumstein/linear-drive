#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32wb55xx.h"
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_LOW ((uint16_t)0)
#define INPUT_HIGH ((uint16_t)65535)

typedef enum
{
    FALLING_EDGE,
    RISING_EDGE,
    NO_EDGE,
} Edge;

typedef enum
{
    LOW_ACTIVE,
    HIGH_ACTIVE,
} ButtonMode;

typedef struct Button_
{
    uint16_t raw_input_set;
    bool input_state;
    Edge last_edge;
    ButtonMode button_mode;
    GPIO_TypeDef *port;
    uint16_t pin;
    void (*sample)(struct Button_ *);
    Edge (*get_edge)(struct Button_ *);
    bool (*get_press)(struct Button_ *);
} Button;

uint8_t button_init(Button *const _this, GPIO_TypeDef *const port, uint16_t pin, ButtonMode mode);

#endif