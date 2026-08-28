#include "button.h"
#include "stm32wbxx_hal.h"

static void get_sample(Button *const _this)
{
    GPIO_PinState current_input_state = GPIO_PIN_SET;
    current_input_state = HAL_GPIO_ReadPin(_this->port, _this->pin);

    _this->raw_input_set = _this->raw_input_set << 1;
    _this->raw_input_set |= current_input_state;

    if (_this->input_state == true)
    {
        // Button pressed
        if (_this->raw_input_set == INPUT_HIGH)
        {
            _this->last_edge = RISING_EDGE;
            _this->input_state = false;
        }
    }
    else
    {
        // Button released
        if (_this->raw_input_set == INPUT_LOW)
        {
            _this->last_edge = FALLING_EDGE;
            _this->input_state = true;
        }
    }
}

static Edge get_edge(Button *const _this)
{
    Edge last_edge = _this->last_edge;
    _this->last_edge = NO_EDGE;
    return last_edge;
}

static bool get_press(Button *const _this)
{
    if (_this->button_mode == LOW_ACTIVE)
    {
        if (_this->get_edge(_this) == FALLING_EDGE)
        {
            return true;
        }
    }
    else
    {
        if (_this->get_edge(_this) == RISING_EDGE)
        {
            return true;
        }
    }
    return false;
}

uint8_t button_init(Button *const _this, GPIO_TypeDef *const port, uint16_t pin, ButtonMode mode)
{
    _this->raw_input_set = 0;
    _this->input_state = false;
    _this->last_edge = NO_EDGE;
    _this->button_mode = mode;
    _this->port = port;
    _this->pin = pin;
    _this->sample = get_sample;
    _this->get_edge = get_edge;
    _this->get_press = get_press;
    return EXIT_SUCCESS;
}