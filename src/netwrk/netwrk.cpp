#include "netwrk.h"
#include "../console/console.h"

#include <Arduino.h>
#include <WiFiNINA.h>


#define SECRET_SSID                         "INTELLIGENT_DEV_MEETUP"
#define SECRET_PASS                         "1234567890"
#define WIFI_TIMEOUT_MS                      10000


bool wifi_enabled = false;


void netwrk_cfg(void)
{
    wifi_enabled = true;
    if (WL_NO_MODULE == WiFi.status())
    {
        wifi_enabled = false;
    }
}

bool netwrk_connect(void)
{
    char ssid[] = SECRET_SSID;
    char pass[] = SECRET_PASS;
    int wifi_status = WL_IDLE_STATUS;

    if (!wifi_enabled) { return false; }

    console_display_str((uint8_t *)"Attempting to connect to SSID: ");
    console_display_str_nl((uint8_t *)ssid);

    const unsigned long max_wait = millis() + (unsigned long)WIFI_TIMEOUT_MS;
    unsigned long curr_time = millis();
    wifi_status = WiFi.begin(ssid, pass);
    while ((wifi_status != WL_CONNECTED) && (max_wait > curr_time))
    {
        delay(1000);
        wifi_status = WiFi.status();
        curr_time = millis();
    }

    if (max_wait < curr_time) { return false; }

    return true;
}

void netwrk_disconnect(void)
{
    if (!wifi_enabled) { return; }

    WiFi.disconnect();
}

IPAddress netwrk_get_ip_addr(void)
{
    IPAddress ip;

    if (!wifi_enabled) { return ip; }

    ip = WiFi.localIP();

    return ip;
}
