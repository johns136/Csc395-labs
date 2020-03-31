#include "common.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "motor.h"
#include "leds.h"
#include "gpio.h"
#include "buttons.h"
#include "scheduler.h"

#ifdef VIRTUAL_SERIAL
#include <VirtualSerial.h>
#else
#warning VirtualSerial not defined, USB IO may not work
#define SetupHardware();
#define USB_Mainloop_Handler();
#endif

#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

volatile int8_t global_m2a;
volatile int8_t global_m2b;

volatile int16_t global_counts_m2 = 0;
volatile int8_t global_error_m2 = 0;

volatile int16_t global_last_m2a_val;
volatile int16_t global_last_m2b_val;

/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void initialize_system(void)
{
  // initalize green and yellow only.
	// initialization defines the IO_structs and sets DDR
	initialize_led(GREEN);
	initialize_led(YELLOW);

	redTask.ready=0;
  redTask.releaseTime =0;
  redTask.period = 0;

  yellowTask.ready=0;
  yellowTask.releaseTime =0;
  yellowTask.period = 0;

  buttonTask.releaseTime = 0;
  buttonTask.period = 0;

  pollKey.ready=0;
  pollKey.releaseTime =0;
  pollKey.period = 0;

  printTask.ready=0;
  printTask.releaseTime =0;
  printTask.period = 0;
	//initialize_led(GREEN);
	//initialize_led(YELLOW);

	// The "sanity check".
	// When you see this pattern of lights you know the board has reset
  SetupHardware();
  light_show();

   //This setups the USB hardware and stdio
}
volatile int ms_ticks;
volatile float wcet;
volatile float wcetFloat;
volatile int now;
volatile int finish;

ISR(TIMER0_COMPA_vect){  //Schedueler?
  ms_ticks = ms_ticks + 1;
}


void encoder_task(){
  #ifdef DEBUG
  ++interrupt_counter;
  #endif

  // Make a copy of the current reading from the encoders
  uint8_t tmpB = PINB;

  // Get value of each channel, making it either a 0 or 1 valued integer
  uint8_t m2a_val = (tmpB & (1 << chA_pin )) >> chA_pin;
  uint8_t m2b_val = (tmpB & (1 << chB_pin )) >> chB_pin;

  // Adding or subtracting counts is determined by how these change between interrupts
  int8_t plus_m2 = m2a_val ^ global_last_m2b_val;
  int8_t minus_m2 = m2b_val ^ global_last_m2a_val;

  // Add or subtract encoder count as appropriate
  if(plus_m2) { global_counts_m2 += 1; }
  if(minus_m2) { global_counts_m2 -= 1; }

  // If both values changed, something went wrong - probably missed a reading
  if(m2a_val != global_last_m2a_val && m2b_val != global_last_m2b_val) {
    global_error_m2 = 1;
    PORTD ^= (1 << PORTD5);
  }

  // Save for next interrupt
  global_last_m2a_val = m2a_val;
  global_last_m2b_val = m2b_val;

}

void read_ms_ticks(){

}
void set_up_ms_timer(){
  USBCON = 0;
  TCCR0A |= (1<< WGM11);
	TCCR0B |= (1<< CS11) | (1<< CS10);
	OCR0A = 250;
	TIMSK0 |= (1<< OCIE0A);
}
void set_up_communication(){

}
int aCounter=0;
void scheduler_task(){
 	int local_ms_ticks = 0;
  if (local_ms_ticks >= redTask.releaseTime){
      redTask.ready =1;
      redTask.releaseTime = ms_ticks + redTask.period;
  }
  if (local_ms_ticks >= yellowTask.releaseTime){
      yellowTask.ready =1;
      yellowTask.releaseTime = ms_ticks + yellowTask.period;
  }
  if(local_ms_ticks >= buttonTask.releaseTime){
    if (is_button_pressed(&_buttonA)){
      ++aCounter;
    }
    buttonTask.releaseTime = ms_ticks + buttonTask.period;
  }
  if(local_ms_ticks >= pollKey.releaseTime){
    pollKey.ready =1;
    pollKey.releaseTime = ms_ticks + pollKey.period;
  }
  if(local_ms_ticks >= printTask.releaseTime){
    printTask.ready =1;
    printTask.releaseTime = ms_ticks + printTask.period;
  }
}

void potentiometer_task(uint8_t channel){
	ADMUX &= 0b11100000;
  if (channel >= 8) {
    ADCSRB |= (1<<MUX5);
    ADMUX |= (channel - 8);
  }
  else {
    ADCSRB &= ~(1<<MUX5);
    ADMUX |= channel;
  }

  // start single conversion
  // write '1' to ADSC
  ADCSRA |= (1<<ADSC);

  // wait for conversion to complete
  // ADSC becomes '0' again
  // till then, run loop continuously
  while(ADCSRA & (1<<ADSC));

}
/****************************************************************************
   MAIN
****************************************************************************/


int main(void) {
  // This prevents the need to reset after flashing
  USBCON = 0;
	uint32_t iterations = 100000;
	initialize_system();
  set_up_ms_timer();
  //set_up_communication;
  SetupHardware();
  sei();

  cli();
  //read_ms_ticks();
  now = ms_ticks;
  sei();
  volatile uint32_t count;
  for (count=0; count < iterations; count++) {
        __asm__ __volatile__("nop");
  }
	cli();
  finish = ms_ticks;
  float empty_wcet = (finish - now);
  _delay_ms(5000);
  printf("finish %d \n\r", (int)finish);
  printf("now %d \n\r", (int)now);

  //read_ms_ticks();
  now = ms_ticks;
  sei();
	//led_on(&_yellow,1);  //let's me know empty for loop worked


  for (count=0; count < iterations; count++) {
      //printf("for loop \n\r");
      encoder_task();
  }
	cli();
  finish = ms_ticks;
  wcet = (finish - now);
  wcetFloat = (wcet - empty_wcet); // 100000.0;
	float wcet_encoder = wcetFloat; //* iterations;


  //read_ms_ticks();
  int now2 = ms_ticks;
  sei();
	//led_on(&_green,1);  //let's me know encoder task worked



  for (count=0; count < iterations; count++) {
      scheduler_task();

  }
	cli();
  int finish2 = ms_ticks;
	float wcet_scheduler = ((finish2 - now2) - empty_wcet); // 100000.0;
	//wcet_scheduler = wcet_scheduler * iterations;


	//read_ms_ticks();
	int now3 = ms_ticks;
	sei();
	//led_off(&_green,1);  //let's me now scheduler worked


	for (count=0; count < iterations; count++) {
      potentiometer_task(9);

  }
	cli();
	int finish3 = ms_ticks;
	float wcet_potentiometer = ((finish3 - now3) - empty_wcet); // 100000.0;
	//wcet_potentiometer = wcet_potentiometer * iterations;
  //time empty for USB_Mainloop_Handler
  // time for loop + task code
  //output wcet results
	sei();
	//led_on(&_green,1);  //let's me now potentiometer worked

  USB_Mainloop_Handler();
  printf("wcet %d \n\r", (int)wcet);
  printf("empty_wcet %d \n\r", (int)empty_wcet);
  printf("wcetFloat %ld \n\r", (long)wcetFloat);
  sei();
  while(1) {
    USB_Mainloop_Handler();
    _delay_ms(10000);
    printf("empty_wcet %d \n\r", (int)empty_wcet);
    printf("wcet_encoder %ld \n\r", (long)wcet_encoder);
		printf("wcet_scheduler %ld \n\r", (long)wcet_scheduler);
		printf("wcet_potentiometer %ld \n\r", (long)wcet_potentiometer);

  } /* end while(1) loop */
} /* end main() */
