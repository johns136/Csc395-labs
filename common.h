#ifndef COMMON_H_
#define COMMON_H_

// Your code will go here in between the guarded block
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <inttypes.h>
#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7
//SET_BIT(DDRB,2) for example will set bit 2 in the DDRB register to 1.
// (Toggle will toggle the value of the bit between 0 and 1.)
#define SET_BIT(reg, pin) (reg |= (1<<pin))
#define CLEAR_BIT(reg, pin) (reg &= ~(1<<pin))
#define TOGGLE_BIT(reg, pin) (reg ^= (1<<pin))

#define CONFIG_OUTPUT(port, pin) (SET_BIT(port,pin))
#define CONFIG_INPUT(port,pin) (CLEAR_BIT(port,pin))

//Note that uint8_t is an unsigned integer of length 8 bits
typedef struct {
  volatile uint8_t * ddr;
  volatile uint8_t * port;
  uint8_t pin;
  volatile uint8_t * portin;
} IO_struct;

#endif
