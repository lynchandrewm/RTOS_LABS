// OS.c
// Matthew DeKoning
// Andrew Lynch
// Feb 2, 2017

#include <stdint.h>
#include "Clock.h"
#include "OS.h"
#include "ADCT2ATrigger.h"
#include "SysTick.h"


uint8_t OS_TimeChange = 0;
uint8_t OS_ADCChange = 0;

static uint8_t OS_TimeIndex;
static uint8_t OS_TimeFlag = 0;

static uint32_t OS_RunTimes[100];

extern void PrintTimeOrDate(void);

void addTime(uint8_t count, uint32_t clock){
  //add milliseconds run time
  uint32_t total = count * 336 + clock *2e-5;
  OS_RunTimes[OS_TimeIndex++] = total;
  if(OS_TimeFlag == 0 && OS_TimeIndex == 100){
    OS_TimeFlag = 1;
    OS_TimeIndex = 0;
  }
  else if(OS_TimeIndex == 100){
    OS_TimeIndex = 0;
  }
}

uint32_t OS_AvgRunTime(){
  uint8_t bound = 0;
  uint32_t total = 0;
  if(OS_TimeFlag){
    bound = 100;
  }
  else{
    bound = OS_TimeIndex;
  }
  for(int i = 0; i < bound; i++){
    total = total+OS_RunTimes[i];
  }
  return total/bound;
}

void OS_Scheduler(void){
  startTimer();
	if(OS_TimeChange){ 
    OS_TimeChange = 0;
    PrintTimeOrDate();
  }
	if(OS_ADCChange){ 
    OS_ADCChange = 0;
    PrintVoltage();
  }
  uint8_t OS_count;
  uint32_t clock = getTimer(&OS_count);
  addTime(OS_count, clock);
  
}