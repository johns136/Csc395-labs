#ifndef GPIO_H_
#include "common.h"
#include <util/delay.h>
#include <inttypes.h>
// These function as enumerated types (using pin numbers as values)
#define GPGREEN 4  //pin 4 on board
#define GPYELLOW 1 //pin 2 on board
#define GPRED 2    // pin 0 on board
#define GPGREEN2 6 // pin 10 on board


// Global variables. They MUST be defined using initialize_led(<COLOR>).
IO_struct _gp_yellow;
IO_struct _gp_red;
IO_struct _gp_green;
IO_struct _gp_green2;

/* initialize the data struct for the specific on-board led.
 */
void initialize_gp_led(int color);

/* Flash the designated on-board led for 250ms on, then 250ms off.
 * Assumes led is initialized */
void flash_gp_led(IO_struct * color, int inverted);

/* Flash all the initialized leds for a sanity check light show */
void gp_light_show();
/* turn on specified on board LED. indicate if inverted (0 turns led on)
*/
void gp_led_on(IO_struct * color, int inverted);

/* turn off specified on board LED. indicate if inverted (1 turns led off)
*/
void gp_led_off(IO_struct * color, int inverted);

/* toggle the led between on and off
*/
void gp_led_toggle(IO_struct * color);
#endif
