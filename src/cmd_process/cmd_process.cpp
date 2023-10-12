#include "cmd_process.h"
#include "../console/console.h"
#include "../ledbar/ledbar.h"
#include "../netwrk/netwrk.h"

#include <string.h>
#include <stdlib.h>

#include <Arduino.h>


static uint8_t cmd_process_str[CONSOLE_MAX_LINE_SZ];
static uint8_t cmd_process_str_len;
static bool cmd_process_do_work;


static void cmd_process_led(char * cmd_idx);
static void cmd_process_led_mode(char * cmd_idx, int led);

static void cmd_process_network(char * cmd_idx);

static uint8_t skip_spaces(char *idx);


void cmd_process_cfg(void)
{
    (void)memset(
        (void *)cmd_process_str,
        0,
        sizeof(uint8_t) * CONSOLE_MAX_LINE_SZ);
    cmd_process_do_work = true;
    cmd_process_str_len = 0u;
}

void cmd_process_load_cmd(
    const uint8_t * const in_cmd,
    uint8_t cmd_len)
{
    (void)memcpy(
        (void *)cmd_process_str,
        (void *)in_cmd,
        (sizeof(uint8_t) * CONSOLE_MAX_LINE_SZ));
    cmd_process_str_len = cmd_len;
    cmd_process_do_work = true;
}

void cmd_process_process_cmd(void)
{
    if ((!cmd_process_do_work) ||
        (0u == cmd_process_str_len))
    {
        return;
    }

    char * cmd_idx = (char *)&cmd_process_str[0];
    cmd_idx += skip_spaces(cmd_idx);

    if ((0 == strncmp(cmd_idx, "help", strlen("help"))) ||
        ((1 == cmd_process_str_len)) && ('?' == *cmd_idx))
    {
        console_display_str_nl((uint8_t *)"Help menu");
        console_display_str_nl((uint8_t *)"Available commands: ");
        console_display_str_nl((uint8_t *)"led");
        console_display_str_nl((uint8_t *)"network");
        console_display_str_nl((uint8_t *)"\r\nAdd '?' to end for more help");
        console_display_str_nl((uint8_t *)"i.e: $ led ?");
    }
    else if (0 == strncmp(cmd_idx, "led", strlen("led")))
    {
        cmd_idx += strlen("led");
        cmd_idx += skip_spaces(cmd_idx);
        cmd_process_led(cmd_idx);
    }
    else if (0 == strncmp(cmd_idx, "network", strlen("network")))
    {
        cmd_idx += strlen("network");
        cmd_idx += skip_spaces(cmd_idx);
        cmd_process_network(cmd_idx);
    }
    else
    {
        console_display_str_nl((uint8_t *)"Command not found");
    }

    cmd_process_do_work = false;
}

static void cmd_process_led(char * cmd_idx)
{
    if ('?' == *cmd_idx)
    {
        console_display_str_nl(
            (uint8_t *)"command format: led <led#>");
        console_display_str_nl(
            (uint8_t *)"  Where:\r\n    <led#> - [0, 1, 2, 3, 4]");
    }
    else if (((char)0x2F < *cmd_idx) && ((char)0x35)> *cmd_idx)
    {
        char led_num_str[2] = { (char)0x0 };
        led_num_str[0] = *cmd_idx;
        int led = atoi(led_num_str);
        cmd_idx += 1;
        cmd_idx += skip_spaces(cmd_idx);
        cmd_process_led_mode(cmd_idx, led);
    }
    else
    {
        console_display_str_nl((uint8_t *)"Syntax error");
    }
}

static void cmd_process_led_mode(char * cmd_idx, int led)
{
    if ('?' == *cmd_idx)
    {
        console_display_str_nl(
            (uint8_t *)"command format: led <led#> <blink|off|on>");
        console_display_str_nl(
            (uint8_t *)"  Where:");
        console_display_str_nl(
            (uint8_t *)"    <blink|off|on>");
        console_display_str_nl(
            (uint8_t *)"      blink - Set the LED to blink");
        console_display_str_nl(
            (uint8_t *)"      off   - Turn the specified LED off");
        console_display_str_nl(
            (uint8_t *)"      on    - Turn the specified LED on");
    }
    else if (0 == strncmp(cmd_idx, "blink", strlen("blink")))
    {
        ledbar_set_led_mode(led, LED_COMMAND_BLINK);
    }
    else if (0 == strncmp(cmd_idx, "off", strlen("off")))
    {
        ledbar_set_led_mode(led, LED_COMMAND_OFF);
    }
    else if (0 == strncmp(cmd_idx, "on", strlen("on")))
    {
        ledbar_set_led_mode(led, LED_COMMAND_ON);
    }
    else
    {
        console_display_str_nl((uint8_t *)"Syntax error");
    }
}

static void cmd_process_network(char * cmd_idx)
{
    if ('?' == *cmd_idx)
    {
        console_display_str_nl(
            (uint8_t *)"command format: network <connect|disconnect#>");
        console_display_str_nl(
            (uint8_t *)"  Where:");
        console_display_str_nl(
            (uint8_t *)"       connect - connect to WiFi network");
        console_display_str_nl(
            (uint8_t *)"    disconnect - disconnect from WiFi network");
        console_display_str_nl(
            (uint8_t *)"          ping - send ping to server");
    }
    else if (0 == strncmp(cmd_idx, "connect", strlen("connect")))
    {
        bool resp = netwrk_connect();
        if (!resp)
        {
            console_display_str_nl((uint8_t *)"Connect: Failed");
        }
        else
        {
            console_display_str_nl((uint8_t *)"Connect: Success");
            IPAddress ip = netwrk_get_ip_addr();
            console_display_ip(ip);
        }
    }
    else if (0 == strncmp(cmd_idx, "disconnect", strlen("disconnect")))
    {
        netwrk_disconnect();
    }
    else if (0 == strncmp(cmd_idx, "ping", strlen("ping")))
    {
        ;
    }
    else
    {
        console_display_str_nl((uint8_t *)"Syntax error");
    }
}

static uint8_t skip_spaces(char *idx)
{
    char *end = idx;

    while (' ' == *end)
    {
        end++;
    }

    return (uint8_t)(end - idx);
}
