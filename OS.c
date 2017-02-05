// OS.c
// Matthew DeKoning
// Andrew Lynch
// Feb 2, 2017

#include <stdint.h>
#include "Clock.h"
#include "OS.h"
#include "ADCT2ATrigger.h"

uint8_t OS_TimeChange = 0;
uint8_t OS_ADCChange = 0;
extern void PrintTimeOrDate(void);

void OS_Scheduler(void){
	if(OS_TimeChange){ 
    OS_TimeChange = 0;
    PrintTimeOrDate();
  }
	if(OS_ADCChange){ 
    OS_ADCChange = 0;
    PrintVoltage();
  }  
}