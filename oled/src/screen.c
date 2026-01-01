// Code for the SH1107 controller
// References:
// https://chatgpt.com/c/6956b548-71f4-8329-91c9-e35036917ddc
// https://www.waveshare.com/wiki/Pico-OLED-1.3
// https://files.waveshare.com/upload/5/58/SH1107Datasheet.pdf
#include "screen.h"
#include "stdio.h"
#include <hardware/gpio.h>

// now() returns time since boot in ms
inline static uint32_t now() { return to_ms_since_boot(get_absolute_time()); }

static void heavy_sleep_ms(uint duration) {
  const uint32_t start = now();
  while (now() - start < duration) {
  };
}

inline static void spi_write(uint8_t Value) {
  spi_write_blocking(SPI_PORT, &Value, 1);
}

inline static void spi_write_bytes(uint8_t pData[], uint32_t Len) {
  spi_write_blocking(SPI_PORT, pData, Len);
}

static void OLED_Reset(void) {
  gpio_put(OLED_PIN_RESET, 1);
  heavy_sleep_ms(50);
  gpio_put(OLED_PIN_RESET, 0);
  heavy_sleep_ms(50);
  gpio_put(OLED_PIN_RESET, 1);
  heavy_sleep_ms(50);
}

static void OLED_WriteReg(uint8_t reg) {
  gpio_put(OLED_PIN_DATA_COMMAND, 0);
  gpio_put(OLED_PIN_CHIPSELECT, 0);
  spi_write(reg);
  gpio_put(OLED_PIN_CHIPSELECT, 1);
}

static void OLED_WriteData(uint8_t data) {
  gpio_put(OLED_PIN_DATA_COMMAND, 1);
  gpio_put(OLED_PIN_CHIPSELECT, 0);
  spi_write(data);
  gpio_put(OLED_PIN_CHIPSELECT, 1);
}

static void OLED_InitReg(void) {
  OLED_WriteReg(0xae); /*turn off OLED display*/

  OLED_WriteReg(0x00); /*set lower column address*/
  OLED_WriteReg(0x10); /*set higher column address*/

  OLED_WriteReg(0xB0); /*set page address*/

  OLED_WriteReg(0xdc); /*set display start line*/
  OLED_WriteReg(0x00);

  OLED_WriteReg(0x81); /*contract control*/
  OLED_WriteReg(0x6f); /*128*/
  OLED_WriteReg(0x21); /* Set Memory addressing mode (0x20/0x21) */

  OLED_WriteReg(0xa0); /*set segment remap*/
  OLED_WriteReg(0xc0); /*Com scan direction*/
  OLED_WriteReg(0xa4); /*Disable Entire Display On (0xA4/0xA5)*/

  OLED_WriteReg(0xa6); /*normal / reverse*/
  OLED_WriteReg(0xa8); /*multiplex ratio*/
  OLED_WriteReg(0x3f); /*duty = 1/64*/

  OLED_WriteReg(0xd3); /*set display offset*/
  OLED_WriteReg(0x60);

  OLED_WriteReg(0xd5); /*set osc division*/
  OLED_WriteReg(0x41);

  OLED_WriteReg(0xd9); /*set pre-charge period*/
  OLED_WriteReg(0x22);

  OLED_WriteReg(0xdb); /*set vcomh*/
  OLED_WriteReg(0x35);

  OLED_WriteReg(0xad); /*set charge pump enable*/
  OLED_WriteReg(0x8a); /*Set DC-DC enable (a=0:disable; a=1:enable) */
}

void OLED_Clear() {
  UWORD column;
  OLED_WriteReg(0xb0); // Set the row  start address
  for (UWORD y = 0; y < OLED_HEIGHT; y++) {
    column = 63 - y;
    // Set column low start address (to low 4 bits)
    OLED_WriteReg(0x00 + (column & 0x0f));
    // Set column high start address (to low always on 4bits (16) + the high 12
    // bits of) column)
    OLED_WriteReg(0x10 + (column >> 4));

    for (UWORD x = 0; x < OLED_WIDTH_BYTES; x++) {
      OLED_WriteData(0x00);
    }
  }
}

static UBYTE reverse(UBYTE temp) {
  temp = ((temp & 0x55) << 1) | ((temp & 0xaa) >> 1);
  temp = ((temp & 0x33) << 2) | ((temp & 0xcc) >> 2);
  temp = ((temp & 0x0f) << 4) | ((temp & 0xf0) >> 4);
  return temp;
}

// sets the controller in column addressing mode at a specific page
void set_page_cmd(uint8_t page) {
  // 0xB = 0b1101 (page set command) + page adddress
  OLED_WriteReg(0xB0 + (page & 0xf));
}

// sets the controller at a specific colum to write bytes to
void set_col_cmd(uint8_t col) {
  // A column is 7 bit, but we can only address 4 bits per command (8 bit)
  // 0x00 (indicates the following 4bits is the lower nibble of the column
  // address) + lower 4 bits of col
  OLED_WriteReg(0x00 + (col & 0x0f));
  // 0x10 indicates the following 4bits are the upper address of column + upper
  // 4 bits of col
  OLED_WriteReg(0x10 + (col >> 4));
}

void OLED_Display(const UBYTE *image) {
  UWORD column, temp;

  set_page_cmd(0);

  for (UWORD y = 0; y < OLED_HEIGHT; y++) {
    column = 63 - y;
    set_col_cmd(column);

    for (UWORD x = 0; x < OLED_WIDTH_BYTES; x++) {
      temp = image[x + y * OLED_WIDTH_BYTES];
      temp = reverse(temp); // reverse the buffer
      // When in data mode, each byte write will auto increment the page and column
      // address registries in the controller
      OLED_WriteData(temp);
    }
  }
}

void OLED_Init() {
  printf("OLED Init\n");

  // SPI Config
  printf("Configuring SPI and pins\n");
  const uint brate = 10000 * 1000;
  spi_init(SPI_PORT, brate);
  gpio_set_function(OLED_PIN_DATA, GPIO_FUNC_SPI);
  gpio_set_function(OLED_PIN_CLOCKIN, GPIO_FUNC_SPI);

  // Initialize pins
  gpio_init(OLED_PIN_RESET);
  gpio_set_dir(OLED_PIN_RESET, GPIO_OUT);

  gpio_init(OLED_PIN_DATA_COMMAND);
  gpio_set_dir(OLED_PIN_DATA_COMMAND, GPIO_OUT);

  gpio_init(OLED_PIN_CHIPSELECT);
  gpio_set_dir(OLED_PIN_CHIPSELECT, GPIO_OUT);

  gpio_set_function(OLED_PIN_BL, GPIO_FUNC_PWM);
  gpio_init(OLED_PIN_BL);
  gpio_set_dir(OLED_PIN_BL, GPIO_OUT);

  gpio_put(OLED_PIN_CHIPSELECT, 1);
  gpio_put(OLED_PIN_DATA_COMMAND, 0);
  gpio_put(OLED_PIN_BL, 1);

  // PWM Config
  printf("Configuring PWM\n");

  const uint slice_num = pwm_gpio_to_slice_num(OLED_PIN_BL);
  pwm_set_wrap(slice_num, 100);
  pwm_set_chan_level(slice_num, PWM_CHAN_B, 1);
  pwm_set_clkdiv(slice_num, 50);
  pwm_set_enabled(slice_num, true);

  // Hardware reset
  printf("resetting screen");
  OLED_Reset();

  // Set the initialization register
  printf("registry init");
  OLED_InitReg();
  heavy_sleep_ms(50);

  // Turn on the OLED display
  OLED_WriteReg(0xaf);

  printf("Initialization complete\n");
}
