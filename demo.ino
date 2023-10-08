#include "src/hrtbt/hrtbt.h"
#include "src/ledbar/ledbar.h"
#include "src/button/button.h"
#include "src/console/console.h"

#include <Arduino.h>

void setup() {
  hrtbt_cfg();
  ledbar_cfg();
  button_switch_cfg();
  console_cfg();
}

void loop() {

  if (BUTTON_STATE_CLOSED == button_state_get(0))
  {
    ledbar_set_led_mode(0, LED_COMMAND_ON);
  }
  else
  {
    ledbar_set_led_mode(0, LED_COMMAND_OFF);
  }

  if (BUTTON_STATE_CLOSED == button_state_get(1))
  {
    ledbar_set_led_mode(1, LED_COMMAND_ON);
  }
  else
  {
    ledbar_set_led_mode(1, LED_COMMAND_OFF);
  }

  if (BUTTON_STATE_CLOSED == button_state_get(2))
  {
    ledbar_set_led_mode(2, LED_COMMAND_ON);
  }
  else
  {
    ledbar_set_led_mode(2, LED_COMMAND_OFF);
  }

  if (BUTTON_STATE_CLOSED == button_state_get(3))
  {
    ledbar_set_led_mode(3, LED_COMMAND_ON);
  }
  else
  {
    ledbar_set_led_mode(3, LED_COMMAND_OFF);
  }

  if (BUTTON_STATE_CLOSED == button_state_get(4))
  {
    ledbar_set_led_mode(4, LED_COMMAND_ON);
  }
  else
  {
    ledbar_set_led_mode(4, LED_COMMAND_OFF);
  }

  hrtbt_action();
  ledbar_action();
  button_action();
  console_action();
}

