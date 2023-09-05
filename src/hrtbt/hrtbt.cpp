#include "hrtbt.h"

#include <Arduino.h>

#include <stdint.h>

#define HRTBT_LED_PIN                       LED_BUILTIN
#define HRTBT_DELAY_MSECS                   100
#define HRTBT_ANLG_IN_PIN                   A0

typedef enum {
    /// @brief
    /// Indicates that the heartbeat LED is on
    HRTBT_LED_ON,
    /// @brief
    /// Indicates that the heartbeat LED is off
    HRTBT_LED_OFF,
} hrtbt_led_state_t;

static unsigned long hrtbt_toggle_time_msecs = 0;
static hrtbt_led_state_t hrtbt_led_state = HRTBT_LED_OFF;

void hrtbt_cfg(void)
{
    pinMode((pin_size_t)HRTBT_LED_PIN, OUTPUT);
}

void hrtbt_action(void)
{
    const unsigned long msecs = millis();

    if (hrtbt_toggle_time_msecs <= msecs)
    {
        if (HRTBT_LED_OFF == hrtbt_led_state)
        {
            // Turn on the led
            digitalWrite((pin_size_t)HRTBT_LED_PIN, HIGH);
            hrtbt_led_state = HRTBT_LED_ON;
        }
        else
        {
            // Turn off the led
            digitalWrite((pin_size_t)HRTBT_LED_PIN, LOW);
            hrtbt_led_state = HRTBT_LED_OFF;
        }

        // Set the next time
        const int val = analogRead((pin_size_t)HRTBT_ANLG_IN_PIN);
        hrtbt_toggle_time_msecs = msecs + (unsigned long)HRTBT_DELAY_MSECS +
                (unsigned long)val;
    }
}
