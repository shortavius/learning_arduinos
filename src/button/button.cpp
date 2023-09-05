#include "button.h"

#include <Arduino.h>

#define BUTTON_COUNT                        5

#define BUTTON_1_PIN                        2
#define BUTTON_2_PIN                        3
#define BUTTON_3_PIN                        4
#define BUTTON_4_PIN                        5
#define BUTTON_5_PIN                        6

#define BUTTON_SAMPLE_DELAY_MSECS           50

#define BUTTON_ISR_PROTO(IDX)                                                  \
static void button_##IDX##_isr(void);
#define BUTTON_ISR_CODE(IDX)                                                   \
static void button_##IDX##_isr(void)                                           \
{                                                                              \
    const int pin_val = digitalRead(buttons[IDX].pin.number);                  \
    if (PIN_ACTIVE_LOW == buttons[IDX].pin.active_type)                        \
    {                                                                          \
        if (pin_val)                                                           \
        {                                                                      \
            buttons[IDX].pin.state = PIN_STATE_DEASSERTED;                     \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            buttons[IDX].pin.state = PIN_STATE_ASSERTED;                       \
        }                                                                      \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        if (pin_val)                                                           \
        {                                                                      \
            buttons[IDX].pin.state = PIN_STATE_ASSERTED;                       \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            buttons[IDX].pin.state = PIN_STATE_DEASSERTED;                     \
        }                                                                      \
    }                                                                          \
}

typedef enum {
    PIN_ACTIVE_LOW,
    PIN_ACTIVE_HIGH,
    PIN_ACTIVE_MAX
} pin_active_types;

typedef enum {
    PIN_STATE_ASSERTED,
    PIN_STATE_DEASSERTED,
    PIN_STATE_MAX
} pin_states;

typedef struct
{
    pin_active_types active_type;
    pin_size_t number;
    pin_states state;
} button_pin;

typedef struct
{
    pin_states debounce[2];
    pin_states debounce_state;
    uint64_t next_sample_time_msecs;
} button_debounce;

typedef struct
{
    button_debounce debounce;
    button_pin pin;
} button_t;

static button_t buttons[BUTTON_COUNT] =
{
    {
        .debounce =
        {
            .debounce = { PIN_STATE_DEASSERTED, PIN_STATE_DEASSERTED },
            .debounce_state = PIN_STATE_DEASSERTED,
            .next_sample_time_msecs = 0u
        },
        .pin =
        {
            .active_type = PIN_ACTIVE_LOW,
            .number = (pin_size_t)BUTTON_1_PIN,
            .state = PIN_STATE_DEASSERTED
        }
    },
    {
        .debounce =
        {
            .debounce = { PIN_STATE_DEASSERTED, PIN_STATE_DEASSERTED },
            .debounce_state = PIN_STATE_DEASSERTED,
            .next_sample_time_msecs = 0u
        },
        .pin =
        {
            .active_type = PIN_ACTIVE_LOW,
            .number = (pin_size_t)BUTTON_2_PIN,
            .state = PIN_STATE_DEASSERTED
        }
    },
    {
        .debounce =
        {
            .debounce = { PIN_STATE_DEASSERTED, PIN_STATE_DEASSERTED },
            .debounce_state = PIN_STATE_DEASSERTED,
            .next_sample_time_msecs = 0u
        },
        .pin =
        {
            .active_type = PIN_ACTIVE_LOW,
            .number = (pin_size_t)BUTTON_3_PIN,
            .state = PIN_STATE_DEASSERTED
        }
    },
    {
        .debounce =
        {
            .debounce = { PIN_STATE_DEASSERTED, PIN_STATE_DEASSERTED },
            .debounce_state = PIN_STATE_DEASSERTED,
            .next_sample_time_msecs = 0u
        },
        .pin =
        {
            .active_type = PIN_ACTIVE_LOW,
            .number = (pin_size_t)BUTTON_4_PIN,
            .state = PIN_STATE_DEASSERTED
        }
    },
    {
        .debounce =
        {
            .debounce = { PIN_STATE_DEASSERTED, PIN_STATE_DEASSERTED },
            .debounce_state = PIN_STATE_DEASSERTED,
            .next_sample_time_msecs = 0u
        },
        .pin =
        {
            .active_type = PIN_ACTIVE_LOW,
            .number = (pin_size_t)BUTTON_5_PIN,
            .state = PIN_STATE_DEASSERTED
        }
    }
};

static void button_update_state(int btn);

void button_switch_cfg(void)
{
    for (
        int idx = 0;
        (int)BUTTON_COUNT > idx;
        ++idx)
    {
        pinMode(buttons[idx].pin.number, INPUT);
    }
}

button_states button_state_get_now(int button)
{
    button_states rv = BUTTON_STATE_MAX;

    if ((int)BUTTON_COUNT > button)
    {
        if (PIN_STATE_ASSERTED == buttons[button].pin.state)
        {
            rv = BUTTON_STATE_CLOSED;
        }
        else
        {
            rv = BUTTON_STATE_OPEN;
        }
    }

    return rv;
}

button_states button_state_get(int button)
{
    button_states rv = BUTTON_STATE_MAX;

    if ((int)BUTTON_COUNT > button)
    {
        if (PIN_STATE_ASSERTED == buttons[button].debounce.debounce_state)
        {
            rv = BUTTON_STATE_CLOSED;
        }
        else
        {
            rv = BUTTON_STATE_OPEN;
        }
    }

    return rv;
}

void button_action(void)
{
    const uint64_t msecs = (uint64_t)millis();

    for (
        int idx = 0;
        (int)BUTTON_COUNT > idx;
        ++idx)
    {
        button_update_state(idx); //BUTTON_SAMPLE_DELAY_MSECS

        if (buttons[idx].debounce.next_sample_time_msecs <= msecs)
        {
            buttons[idx].debounce.debounce[1] =
                                            buttons[idx].debounce.debounce[0];
            buttons[idx].debounce.debounce[0] = buttons[idx].pin.state;

            if (buttons[idx].debounce.debounce[1] ==
                                            buttons[idx].debounce.debounce[0])
            {
                buttons[idx].debounce.debounce_state = buttons[idx].pin.state;
            }

            buttons[idx].debounce.next_sample_time_msecs = msecs +
                                        (uint64_t)BUTTON_SAMPLE_DELAY_MSECS;
        }
    }
}

static void button_update_state(int btn)
{
    if ((int)BUTTON_COUNT > btn)
    {
        const int pin_val = digitalRead(buttons[btn].pin.number);

        if (PIN_ACTIVE_LOW == buttons[btn].pin.active_type)
        {
            if (pin_val)
            {
                buttons[btn].pin.state = PIN_STATE_DEASSERTED;
            }
            else
            {
                buttons[btn].pin.state = PIN_STATE_ASSERTED;
            }
        }
        else
        {
            if (pin_val)
            {
                buttons[btn].pin.state = PIN_STATE_ASSERTED;
            }
            else
            {
                buttons[btn].pin.state = PIN_STATE_DEASSERTED;
            }
        }
    }
}
