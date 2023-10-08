#include "console.h"
#include "../cmd_process/cmd_process.h"

#include <string.h>

#include <Arduino.h>

#define CONSOLE_PORT                        Serial
#define CONSOLE_BAUDRATE                    115200


static uint8_t cmd_byte_pos = 0u;
static uint8_t cmd_str[CONSOLE_MAX_LINE_SZ + 1] = { 0u };


static void display_prompt(void);
static void display_delete_character(void);


void console_cfg(void)
{
    CONSOLE_PORT.begin((long)CONSOLE_BAUDRATE);
    display_prompt();
}

void console_display_str_nl(const uint8_t * const str)
{
    CONSOLE_PORT.println((char *)str);
}

void console_action(void)
{
    if (0 < CONSOLE_PORT.available())
    {
        const uint8_t in_byte = (uint8_t)(0xFF & CONSOLE_PORT.read());

        if ((uint8_t)0x08 == in_byte)
        {
            if ((uint8_t)0 != cmd_byte_pos)
            {
                display_delete_character();
                cmd_byte_pos--;
                cmd_str[cmd_byte_pos] = (uint8_t)0u;
            }
        }
        else if ((uint8_t)0x0D == in_byte)
        {
            cmd_str[cmd_byte_pos] = (uint8_t)0u;
            CONSOLE_PORT.print("\r\n");
            cmd_process_load_cmd(cmd_str, cmd_byte_pos);
            cmd_process_process_cmd();
            (void)memset(
                (void *)&cmd_str,
                0x00,
                (sizeof(uint8_t) * CONSOLE_MAX_LINE_SZ));
                cmd_byte_pos = 0u;
                display_prompt();
        }
        else if (((uint8_t)0x1F < in_byte) && ((uint8_t)0x7F > in_byte))
        {
            if ((uint8_t)CONSOLE_MAX_LINE_SZ > cmd_byte_pos)
            {
                cmd_str[cmd_byte_pos] = in_byte;
                cmd_byte_pos++;
                CONSOLE_PORT.print((char)in_byte);
            }
        }
    }
}

static void display_prompt(void)
{
    CONSOLE_PORT.print("\r\n$ ");
}

static void display_delete_character(void)
{
    CONSOLE_PORT.print("\b \b");
}
