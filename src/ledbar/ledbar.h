#if !defined(LEDBAR_H)
#define LEDBAR_H

#include <stdint.h>

typedef enum {
    LED_COMMAND_ON,
    LED_COMMAND_OFF,
    LED_COMMAND_BLINK,
    LED_COMMAND_MAX
} led_commands;

void ledbar_cfg(void);
void ledbar_set_led_mode(
    int led,
    led_commands cmd);
void ledbar_set_blink_rate_msecs(
    int led,
    uint64_t delay);
void ledbar_action();

#endif // LEDBAR_H
