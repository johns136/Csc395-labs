#ifndef SCHEDULER_H_
#define SCHEDULER_H_


#include "common.h"
#include <util/delay.h>
#include <inttypes.h>

typedef struct {
  volatile uint8_t ready;
  volatile uint32_t releaseTime;
  volatile uint32_t period;
} task_struct;

task_struct redTask;
task_struct yellowTask;
task_struct buttonTask;
task_struct pollKey;
task_struct printTask;

void task_toggleGreen();
void task_toggleRed();
void task_toggleYellow();
//void check_buttonPress(IO_struct * button);

#endif
