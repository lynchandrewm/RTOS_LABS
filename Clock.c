// Clock.c
// Runs on TM4C123
// Use Timer4B in periodic mode to create clock functions
// Andrew Lynch
// January 31, 2017

#include <stdint.h>
#include <stdio.h>
#include "Timer5A.h"
#include "../inc/tm4c123gh6pm.h"

void UpdateTime(void);

uint16_t static Year = 0;
uint8_t static LeapYear = 0;
uint8_t static Month = 0;
uint8_t static DaysInMonth = 0;
uint8_t static Day = 0;
uint8_t static Hour = 0;
uint8_t static Minute = 0;
uint8_t static Second = 0;
uint16_t static Millisecond = 0;

#define MILLISECOND    0x0000001F
#define SECOND         0x0001F800
#define MINUTE         0x01F80000
#define HOUR           0xF8000000
#define DAY            0x0000001F
#define DAYS_PER_MONTH 0x000007C0
#define MONTH          0x0000F000
#define YEAR           0x3FFE0000
#define LEAPYEAR_EN    0x80000000


//milliseconds            bits 0-8
//seconds                 bits 11-16
//minutes                 bits 19-24
//hours                   bits 27-31
uint32_t static Time = 0x00000000;

//day                     bits 0-4
//days in current month   bits 6-10
//month                   bits 12-15
//year                    bits 17-29
//leap year enable        bit  31
uint32_t static Date = 0x00000000;

uint16_t ExtractMillisecond(void){
  return (Time&MILLISECOND);
}

uint8_t ExtractSecond(void){
  return (Time&SECOND)>>11;
}

uint8_t ExtractMinute(void){
  return (Time&MINUTE)>>19;
}

uint8_t ExtractHour(void){
  return (Time&HOUR)>>27;
}

uint8_t ExtractDay(void){
  return (Date&DAY);
}

uint8_t ExtractDaysInCurrentMonth(void){
  return (Date&DAYS_PER_MONTH)>>6;
}

uint8_t ExtractMonth(void){
  return (Date&MONTH)>>12;
}

uint16_t ExtractYear(void){
  return (Date&YEAR)>>17;
}

uint8_t ExtractLeapYearEnable(void){
  return (Date&LEAPYEAR_EN)>>31;
}



void Clock_GetTime(char string[12]){
  char* AMPMstr = "AM";
  char* leadingZeroHour = "";
  char* leadingZeroMinute = "";
  char* leadingZeroSecond = "";
  uint8_t hour = Hour;
  if(Hour>11){ 
    AMPMstr = "PM";
    hour -= 12;
  }
  if(hour == 0){ hour = 12; }
  if(hour<10){ leadingZeroHour = "0"; }
  if(Minute<10){ leadingZeroMinute = "0"; }
  if(Second<10){ leadingZeroSecond = "0"; }
  sprintf(string, "%s%d:%s%d:%s%d %s", leadingZeroHour, hour, leadingZeroMinute, Minute, leadingZeroSecond, Second, AMPMstr);
}

void NameOfMonth(char* string){
  switch(Month){
    case 1: 
      sprintf(string, "January");
      break;
    case 2:
      sprintf(string, "February");
      break;
    case 3: 
      sprintf(string, "March");
      break;
    case 4: 
      sprintf(string, "April");
      break;
    case 5: 
      sprintf(string, "May");
      break;
    case 6: 
      sprintf(string, "June");
      break;
    case 7: 
      sprintf(string, "July");
      break;
    case 8: 
      sprintf(string, "August");
      break;
    case 9: 
      sprintf(string, "September");
      break;
    case 10: 
      sprintf(string, "October");
      break;
    case 11: 
      sprintf(string, "November");
      break;
    case 12: 
      sprintf(string, "December");
      break;
  }
}

void Clock_GetDate(char string[19]){
  char monthName[10];
  NameOfMonth(monthName);
  sprintf(string, "%s %d, %d", monthName, Day, Year); 
}  



void Timer5A_Handler(void){
  TIMER5_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER4B timeout
  Millisecond++;
  UpdateTime();
}

void UpdateDaysInMonth(void){
  if(LeapYear&&(Month==2)){
    DaysInMonth = 29;
    return;
  }
  DaysInMonth = 28 + (Month + Month/8) % 2 + 2 % Month + 2 * 1/Month;
}

void UpdateLeapYear(void){
  if(!(Year%4)){
    LeapYear = 0;
    return;
  }
  if(!(Year%100)){
    LeapYear = 1;
    return;
  }
  if(!(Year%400)){
    LeapYear = 0;
    return;
  }
}

void UpdateTime(void){
  if(Millisecond == 1000){
    Second++; 
    Millisecond = 0;
  }
  if(Second == 60){
    Minute++;
    Second = 0;
  }
  if(Minute == 60){
    Hour++;
    Minute = 0;
  }
  if(Hour == 24){
    Day++;
    Hour = 0;
  }
  if(Day > DaysInMonth){
    Month++;
    UpdateDaysInMonth();
    Day = 1;
  }
  if(Month > 12){
    Year++;
    UpdateLeapYear();
    Month = 1;
  }
}

void Clock_Init(void){
  Timer5A_Init(50000); //1ms 
}

void Clock_SetClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second){
  Year = year; Month = month; Day = day; Hour = hour; Minute = minute; Second = second; 
  Millisecond = 0;
  UpdateDaysInMonth();
  UpdateLeapYear();
}

void Clock_SetYear(uint16_t year){
  Year = year;
  UpdateLeapYear();
}

void Clock_SetMonth(uint8_t month){
  Month = month;
  UpdateDaysInMonth();
}

void Clock_SetDay(uint8_t day){
  Day = day;
}

void Clock_SetHour(uint8_t hour){
  Hour = hour;
}

void Clock_SetMinute(uint8_t minute){
  Minute = minute;
}

void Clock_SetSecond(uint8_t second){
  Second = second;
  Millisecond = 0;
}

