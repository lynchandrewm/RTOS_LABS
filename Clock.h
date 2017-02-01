// Clock.h
// Runs on TM4C123
// Use Timer4B in periodic mode to create clock functions
// Andrew Lynch
// January 31, 2017

void Clock_GetTime(char string[12]);

void Clock_GetDate(char string[19]);

void Clock_Init(void);

void Clock_SetClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

void Clock_SetYear(uint16_t year);

void Clock_SetMonth(uint8_t month);

void Clock_SetDay(uint8_t day);

void Clock_SetHour(uint8_t hour);

void Clock_SetMinute(uint8_t minute);

void Clock_SetSecond(uint8_t second);


