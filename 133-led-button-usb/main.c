#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"

const uint LED_PIN = 25;
const uint BUTTON_PIN = 15;
int DEBOUNCE_MS = 20;

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);
}

void set_led(bool on)
{
    gpio_put(LED_PIN, on);
    printf("led %s\n", on ? "on" : "off");
}

bool handle_command(int command, bool led)
{
    if (command == 'e')
    {
        led = true;
        set_led(led);
    }
    else if (command == 'd')
    {
        led = false;
        set_led(led);
    }
    else
    {
        printf("unknown command: %c\n", command);
    }

    return led;
}

int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    bool led_state = 0;
    bool previos_led_state = 0;

    while (1)
    {
        led_state = get_button_debounce(BUTTON_PIN);
        if(led_state != previos_led_state){
            set_led(led_state);
            previos_led_state = led_state;
        }

        int command = getchar_timeout_us(0);

        if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }

        led_state = handle_command(command, led_state);
        
    }
}