// OS.h
// Matthew DeKoning
// Andrew Lynch

#ifndef _OS
#define _OS

extern uint8_t OS_ADCChange;
extern uint8_t OS_TimeChange;

void OS_Scheduler(void);

uint32_t OS_AvgRunTime();

uint32_t OS_LongRunTime();

uint32_t OS_ShortRunTime();
#endif //_OS