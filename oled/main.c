#include "pico/cyw43_arch.h"
#include "src/draw.h"
#include "src/screen.h"
#include "stdio.h"
#include <pico/time.h>
#include <stdlib.h>
#include <string.h>

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

  OLED_Init();
  OLED_Clear();

  UBYTE *image = {0};
  UWORD size = OLED_WIDTH_BYTES * OLED_HEIGHT;
  if ((image = (UBYTE *)malloc(size)) == NULL) {
    while (1) {
      printf("malloc failed \n");
      sleep_ms(500);
    }
  }

  bool led = true;
  pico_set_led(led);

  while (true) {
    int start = time_us_64();

    draw_text(image, 20, 20, "coucou amelie <3");
    draw_text(image, 20, 40, "woooooooooooooooooooooooooo!!");
    OLED_Display(image);
    memset(image, 0, size);

    uint64_t delta = (time_us_64() - start);
    sleep_us(33333 * 2 - delta);

    led = !led;
    pico_set_led(led);
  }
}
