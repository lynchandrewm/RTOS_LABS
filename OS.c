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

static uint16_t OS_TimeIndex;
static uint8_t OS_TimeFlag = 0;

static uint32_t OS_RunTimes[1000];

static uint32_t OS_Longest = 0;
static uint32_t OS_Shortest = 0xFFFFFFFF;

extern void PrintTimeOrDate(void);

void addTime(uint8_t count, uint32_t clock){
  //add nanoseconds run time
  uint32_t total = count * 33554430 + clock*20;
  OS_RunTimes[OS_TimeIndex++] = total;
  if(OS_TimeFlag == 0 && OS_TimeIndex == 1000){
    OS_TimeFlag = 1;
    OS_TimeIndex = 0;
  }
  else if(OS_TimeIndex == 1000){
    OS_TimeIndex = 0;
  }
  if(total < OS_Shortest){
    OS_Shortest = total;
  }
  if(total > OS_Longest){
    OS_Longest = total;
  }
}

uint32_t OS_AvgRunTime(){
  uint16_t bound = 0;
  uint32_t total = 0;
  if(OS_TimeFlag){
    bound = 1000;
  }
  else{
    bound = OS_TimeIndex;
  }
  for(int i = 0; i < bound; i++){
    total = total+OS_RunTimes[i];
  }
  return total/bound;
}

uint32_t OS_LongRunTime(){
  return OS_Longest;
}

uint32_t OS_ShortRunTime(){
  return OS_Shortest;
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