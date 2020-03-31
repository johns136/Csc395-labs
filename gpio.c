#include "gpio.h"

/* Scope is limited to this file. Used to indicate if LED is initialized. */
static uint8_t bYellowInit = 0;
static uint8_t bRedInit = 0;
static uint8_t bGreenInit = 0;
static uint8_t bGreenInit2 = 0;


/* configure the data direction for the specified on-board led.
 */
void configure_gpled(IO_struct * color) {
  SET_BIT(*color->ddr, color->pin);
}

void initialize_gp_led(int color) {
  switch(color) {
    case (GPYELLOW) :
    _gp_yellow = (IO_struct) { &DDRD, &PORTD, GPYELLOW, &PIND };
    configure_gpled(&_gp_yellow);
    bYellowInit = 1;
    break;

    case(GPGREEN):
    _gp_green = (IO_struct) { &DDRD, &PORTD, GPGREEN, &PIND };
    configure_gpled(&_gp_green);
    bGreenInit = 1;
    break;

    case(GPRED):
    _gp_red = (IO_struct) { &DDRD, &PORTD, GPRED, &PIND };
    configure_gpled(&_gp_red);
    bRedInit = 1;
    break;

    case(GPGREEN2):
    _gp_green2 = (IO_struct) { &DDRB, &PORTB, GPGREEN2, &PINB };
    configure_gpled(&_gp_green2);
    bGreenInit2 = 1;
    break;
  }
}

void gp_led_on(IO_struct * color, int inverted) {
  if (inverted) CLEAR_BIT(*color->port, color->pin);
  else SET_BIT(*color->port, color->pin);
}

void gp_led_off(IO_struct * color, int inverted) {
  if (inverted) SET_BIT(*color->port, color->pin);
  else CLEAR_BIT(*color->port, color->pin);
}

void gp_led_toggle(IO_struct * color) {
  TOGGLE_BIT(*color->port, color->pin);
}

/* Flash the designated on-board led for 250ms on, then 250ms off.
 * Assumes led is initialized */
void flash_gp_led(IO_struct * color, int inverted) {
  if (!inverted) {
    SET_BIT(*color->port, color->pin);
  } else {
    CLEAR_BIT(*color->port, color->pin);
  }
  _delay_ms(100);
  TOGGLE_BIT(*color->port, color->pin);
  _delay_ms(100);
}

/* Flash all the initialized leds for a sanity check light show */
void gp_light_show() {
  int i;
  for (i = 0; i < 2; i++) {
    if (bYellowInit) flash_gp_led(&_gp_yellow, 0);  // not inverted (1 turns led on)
    if (bRedInit) flash_gp_led(&_gp_red, 0);  // inverted (0 turns led on)
    if (bGreenInit) flash_gp_led(&_gp_green, 0);
    if (bGreenInit2) flash_gp_led(&_gp_green2, 0);
  }
}
