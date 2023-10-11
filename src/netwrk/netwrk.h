#if !defined(NETWRK_H)
#define NETWRK_H


#include <IPAddress.h>

#include <stdbool.h>


void netwrk_cfg(void);
bool netwrk_connect(void);
void netwrk_disconnect(void);
IPAddress netwrk_get_ip_addr(void);


#endif // NETWRK_H
