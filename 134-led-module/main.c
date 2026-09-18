#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"
#include "led.h"

const uint BUTTON_PIN = 15;
int DEBOUNCE_MS = 20;

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);
}

void handle_command(int command)
{
    if (command == 'e')
    {
        led_set(true);
    }
    else if (command == 'd')
    {
        led_set(false);
    }
    else
    {
        printf("unknown command: %c\n", command);
    }

}

int main()
{
    stdio_init_all();
    led_init();
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    bool previos_led_state = 0;
    bool led_state = 0;

    while (1)
    {
        led_state = get_button_debounce(BUTTON_PIN);
        if(led_state == false && previos_led_state == true){
            led_toggle();
            printf("led %s\n", led_is_on() ? "on" : "off");
            previos_led_state = led_state;
        }

        int command = getchar_timeout_us(0);

        if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }

        handle_command(command);
        
    }
}