#ifndef LED_H
#define LED_H

#include "pico/stdlib.h"

void led_init(void);
void led_set(bool on);
void led_toggle(void);
bool led_is_on(void);

#endif