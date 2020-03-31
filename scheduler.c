#include "gpio.h"
#include "scheduler.h"
#include "buttons.h"

void task_toggleGreen(){
  gp_led_toggle(&_gp_green);
}
void task_toggleYellow(){
  gp_led_toggle(&_gp_yellow);
}
void task_toggleRed(){
  gp_led_toggle(&_gp_red);
}
/*void check_buttonPress(IO_struct * button){
  is_button_pressed(IO_struct * button);
}*/
