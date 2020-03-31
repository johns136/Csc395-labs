#include "timers.h"



void setup_timer_zero(){

}

void setup_timer_one(){
  TCCR1B |= (1<< WGM12);
  TCCR1B |= (1<< CSN2);
}

void setup_timer_two(){

}
