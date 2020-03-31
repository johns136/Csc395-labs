#ifndef TIMERS_H_
#include "common.h"
#include <util/delay.h>
#include <inttypes.h>

#include "avr/interrupts"

#define WGM0
#define WGM1

void setup_timer_zero();

void setup_timer_one();

void setup_timer_two();


#endif
