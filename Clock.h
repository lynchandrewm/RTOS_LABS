// Clock.h
// Runs on TM4C123
// Use Timer4B in periodic mode to create clock functions
// Andrew Lynch
// January 31, 2017

void Clock_GetTime(char string[12]);

void Clock_GetDate(char string[19]);

void Clock_Init(void);

void Clock_SetClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);



