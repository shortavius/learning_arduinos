#include "src/hrtbt/hrtbt.h"
#include "src/ledbar/ledbar.h"
#include "src/button/button.h"
#include "src/console/console.h"
#include "src/cmd_process/cmd_process.h"

#include <Arduino.h>

void setup() {
  hrtbt_cfg();
  ledbar_cfg();
  button_switch_cfg();
  console_cfg();
  cmd_process_cfg();
}

void loop() {
  hrtbt_action();
  ledbar_action();
  button_action();
  console_action();
}

