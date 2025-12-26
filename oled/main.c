#include "pico/cyw43_arch.h"
#include <pico/stdlib.h>
#include <stdio.h>

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

void pico_set_led(bool led_on) {
  // Ask the wifi "driver" to set the GPIO on or off
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
}

int main() {
  // init stdio
  stdio_init_all();

  while (true) {
    printf("Led ON\n");
    pico_set_led(true);
    sleep_ms(LED_DELAY_MS);

    printf("Led OFF\n");
    pico_set_led(false);
    sleep_ms(LED_DELAY_MS);
  }
}

/*
#include <malloc.h>
void print_memory_usage() {
    struct mallinfo info = mallinfo();

    printf("Total allocated: %d bytes\n", info.uordblks);
    printf("Total free: %d bytes\n", info.fordblks);
    printf("Total heap size: %d bytes\n", info.arena);
    printf("Largest free block: %d bytes\n", info.ordblks);
}
*/
