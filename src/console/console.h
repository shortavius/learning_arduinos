#if !defined(CONSOLE_H)
#define CONSOLE_H


#include <IPAddress.h>

#include <stdint.h>


#define CONSOLE_MAX_LINE_SZ                 80


void console_cfg(void);
void console_display_ip(IPAddress ip);
void console_display_str(const uint8_t * const str);
void console_display_str_nl(const uint8_t * const str);
void console_action(void);

#endif /* CONSOLE_H */
