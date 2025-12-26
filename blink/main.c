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

  // use the wifi driver led, initialize the driver
  int init;
  do {
    init = cyw43_arch_init();
    printf("init code %d\n", init);
    sleep_ms(1000);
  } while (init != PICO_OK);
  printf("init code ok\n");

  while (true) {
    printf("Led ON\n");
    pico_set_led(true);
    sleep_ms(LED_DELAY_MS);

    printf("Led OFF\n");
    pico_set_led(false);
    sleep_ms(LED_DELAY_MS);
  }
}
