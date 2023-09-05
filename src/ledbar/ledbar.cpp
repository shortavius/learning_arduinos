#include "ledbar.h"

#include <Arduino.h>

#define BAR_LED_COUNT                       5

#define LED_BAR_1_PIN                       12
#define LED_BAR_2_PIN                       11
#define LED_BAR_3_PIN                       10
#define LED_BAR_4_PIN                       9
#define LED_BAR_5_PIN                       8

#define LED_DEFAULT_BLINK_MSECS             500

typedef enum {
    PIN_ACTIVE_LOW,
    PIN_ACTIVE_HIGH,
    PIN_ACTIVE_MAX
} pin_active_types;

typedef enum {
    PIN_STATE_ON,
    PIN_STATE_OFF,
    PIN_STATE_MAX
} pin_states;

typedef struct
{
    pin_active_types active_type;
    pin_size_t number;
    pin_states state;
} led_pin;

typedef enum
{
    LED_MODE_STATIC,
    LED_MODE_BLINK,
    LED_MODE_MAX
} led_modes;

typedef struct
{
    led_modes mode;
    led_pin pin;
    uint64_t delay_msecs;
    uint64_t next_toggle_time_msecs;
} led_t;


static led_t ledbar_leds[BAR_LED_COUNT] =
{
    {
        .mode = LED_MODE_STATIC,
        .pin =
        {
            .active_type = PIN_ACTIVE_LOW,
            .number = LED_BAR_1_PIN,
            .state = PIN_STATE_MAX
        },
        .delay_msecs = (uint64_t)LED_DEFAULT_BLINK_MSECS,
        .next_toggle_time_msecs = (uint64_t)0
    },
    {
        .mode = LED_MODE_STATIC,
        .pin =
        {
            .active_type = PIN_ACTIVE_LOW,
            .number = LED_BAR_2_PIN,
            .state = PIN_STATE_MAX
        },
        .delay_msecs = (uint64_t)LED_DEFAULT_BLINK_MSECS,
        .next_toggle_time_msecs = (uint64_t)0
   },
    {
        .mode = LED_MODE_STATIC,
        .pin =
        {
            .active_type = PIN_ACTIVE_LOW,
            .number = LED_BAR_3_PIN,
            .state = PIN_STATE_MAX
        },
        .delay_msecs = (uint64_t)LED_DEFAULT_BLINK_MSECS,
        .next_toggle_time_msecs = (uint64_t)0
    },
    {
        .mode = LED_MODE_STATIC,
        .pin =
        {
            .active_type = PIN_ACTIVE_LOW,
            .number = LED_BAR_4_PIN,
            .state = PIN_STATE_MAX
        },
        .delay_msecs = (uint64_t)LED_DEFAULT_BLINK_MSECS,
        .next_toggle_time_msecs = (uint64_t)0
    },
    {
        .mode = LED_MODE_STATIC,
        .pin =
        {
            .active_type = PIN_ACTIVE_LOW,
            .number = LED_BAR_5_PIN,
            .state = PIN_STATE_MAX
        },
        .delay_msecs = (uint64_t)LED_DEFAULT_BLINK_MSECS,
        .next_toggle_time_msecs = (uint64_t)0
    }
};

static void pin_assert(led_pin * p_pin);
static void pin_deassert(led_pin * p_pin);

void ledbar_cfg(void)
{
    for(
       int idx = 0;
       (int)BAR_LED_COUNT > idx;
       ++idx)
    {
        pinMode(ledbar_leds[idx].pin.number, OUTPUT);
        pin_deassert(&ledbar_leds[idx].pin);
    }
}

void ledbar_set_led_mode(
    int led,
    led_commands cmd)
{
    if ((int)BAR_LED_COUNT <= led) { return; }

    if (LED_COMMAND_BLINK == cmd)
    {
        ledbar_leds[led].mode = LED_MODE_BLINK;
    }
    else
    {
        if (ledbar_leds[led].mode != LED_MODE_STATIC)
        {
            ledbar_leds[led].mode = LED_MODE_STATIC;
        }

        if (LED_COMMAND_ON == cmd)
        {
            pin_assert(&ledbar_leds[led].pin);
        }
        else
        {
            pin_deassert(&ledbar_leds[led].pin);
        }
    }
}

void ledbar_set_blink_rate_msecs(
    int led,
    uint64_t delay)
{
    if ((int)BAR_LED_COUNT <= led) { return; }

    ledbar_leds[led].delay_msecs = delay;
}

void ledbar_action()
{
    const uint64_t curr_time = millis();

    for (
        int idx = 0u;
        (int)BAR_LED_COUNT > idx;
        ++idx)
    {
        if ((LED_MODE_STATIC == ledbar_leds[idx].mode) ||
           (ledbar_leds[idx].next_toggle_time_msecs > curr_time)) { continue; }

        if (PIN_STATE_ON == ledbar_leds[idx].pin.state)
        {
            pin_deassert(&ledbar_leds[idx].pin);
        }
        else
        {
            pin_assert(&ledbar_leds[idx].pin);
        }

        ledbar_leds[idx].next_toggle_time_msecs +=
                                                ledbar_leds[idx].delay_msecs;
    }
}

static void pin_assert(led_pin * p_pin)
{
    if (PIN_ACTIVE_LOW == p_pin->active_type)
    {
        digitalWrite(p_pin->number, LOW);
    }
    else
    {
        digitalWrite(p_pin->number, HIGH);
    }
    p_pin->state = PIN_STATE_ON;
}

static void pin_deassert(led_pin * p_pin)
{
    if (PIN_ACTIVE_LOW == p_pin->active_type)
    {
        digitalWrite(p_pin->number, HIGH);
    }
    else
    {
        digitalWrite(p_pin->number, LOW);
    }
    p_pin->state = PIN_STATE_OFF;
}
