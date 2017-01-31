#include "string.h"
#include "interpreter.h"

static char strArray[ROWS][COLS];

int8_t interpreter_line = 0;
int8_t interpreter_device = 0;
char interpreter_msg[21];
/* the error message when an invalid command was passed */
const char static * errorMsg = "Invalid Command";


void INTERPRETER_initArray(){
  int i;
  for(i = 0; i < ROWS; i++){
    strArray[i][0] = '\0';
  }
}
void INTERPRETER_clearMsg(){
  int i;
  for(i = 0; i < 21; i++)
    interpreter_msg[i] = '\0';
}

void INTERPRETER_handleMessage(char* str){
  INTERPRETER_clearMsg();
  const char* delim = " ";
  interpreter_device = 0; interpreter_line = 0;
  strtoklist((const char*)str, strArray , delim);
  //INTERPRETER_printParams();
  int command = INTERPRETER_handleCommand(INTERPRETER_handleParam1(&interpreter_device, &interpreter_line));
  
  //int screenFlag = 1;
  //if(interpreter_device == -1){
    //screenFlag = 0;
  //}
  //ST7735_ds_SetCursor(interpreter_device, 0, interpreter_line);
  int i = 0;
  switch(command){
    case 1:
      //calculator
      interpreter_msg[0] = 'A';interpreter_msg[1] = 'N';interpreter_msg[2] = 'S';interpreter_msg[3] = ' ';interpreter_msg[4] = '=';interpreter_msg[5] = ' ';
      itoa(INTERPRETER_calculator(), interpreter_msg, 10, 6);
      break;
    case 2:
      //adc
      interpreter_msg[0] = 'A';interpreter_msg[1] = 'D';interpreter_msg[2] = 'C';interpreter_msg[3] = ' ';interpreter_msg[4] = '=';interpreter_msg[5] = ' ';
      //itoa(GET_ADC(), interpreter_msg, 10, 6);
      break;
    case 3:
      //timer
      interpreter_msg[0] = 'T';interpreter_msg[1] = 'i';interpreter_msg[2] = 'm';interpreter_msg[3] = 'e';interpreter_msg[4] = ':';interpreter_msg[5] = ' ';
      //itoa(GET_TIMER(), interpreter_msg, 10, 6);
      break;
    case 4:
      //print string
      for(i = 0; i < LengthOfString(strArray[1]);i++){
        interpreter_msg[i] = strArray[1][i];
      }
	  interpreter_msg[i+1] = '\0';
      break;
    case 5:
      //adjust screen
      //ST7735_ds_InitR(INITR_REDTAB, strToInt(strArray[1]), strToInt(strArray[2]), strToInt(strArray[3]),strToInt(strArray[4]));
      break;
    default:
      for(i = 0; i < LengthOfString(errorMsg); i++){
        interpreter_msg[i] = errorMsg[i];
      }
      break;
  }
  
}

int32_t INTERPRETER_calculator(){
  int i;
  for(i = 0; i < ROWS; i++){
    if(strArray[i][0] == '\0')
      break;
  }
  int total = i;
  i = 0;
  int stack[i];
  int index = 0;
  while(i < total){
    uint8_t op = INTERPRETER_detectOperator(strArray[i][0]);
    if(op == 0xff || ((int)strArray[i][1] >=48 && (int)strArray[i][1] <= 57)){
      stack[index] = strToInt(strArray[i]);
      index++;
    }
    else{
      if(index >= 2){
        int value = 0;
        switch (op){
          case 0:
            value = stack[index-2] + stack[index-1];
            stack[index] = 0;
            index--;
            stack[index-1] = value;
            break;
          case 1:
            value = stack[index-2] - stack[index-1];
            stack[index] = 0;
            index--;
            stack[index-1] = value;
            break;
          case 2:
            value = stack[index-2] * stack[index-1];
            stack[index] = 0;
            index--;
            stack[index-1] = value;
            break;
          case 3:
            value = stack[index-2] / stack[index-1];
            stack[index] = 0;
            index--;
            stack[index-1] = value;
            break;
        }
      }
      else{
        return 0;
      }
    }
    i++;
  }
  return stack[0];
}
void INTERPRETER_fixArray0(uint8_t index){
  int i;
  for(i = 0; i < (LengthOfString(strArray[0]) - index); i++){
    strArray[0][i] = strArray[0][index+i];
  }
  strArray[0][i] = '\0';
}
uint8_t INTERPRETER_handleCommand(uint8_t index){
  int num = 1;
  int i;
  for(i = 0; i < (LengthOfString(strArray[0]) - index); i++){
    if(((int)strArray[0][index+i] <48 || (int)strArray[0][index+i] > 57) && strArray[0][index+i] != '-' ){
      num = 0;
      break;
    }
  }
  if(num){
    INTERPRETER_fixArray0(index);
    return 1;
  }
  switch (LengthOfString(strArray[0]) - index){
    case 3:
      if(strArray[0][index] == 'a' && strArray[0][index+1] == 'd' && strArray[0][index+2] == 'c'){
        return 2;
      }
      return 0xff;
      break;
    case 5:
      if(strArray[0][index] == 't' && strArray[0][index+1] == 'i' && strArray[0][index+2] == 'm' && strArray[0][index+3] == 'e' && strArray[0][index+4] == 'r'){
        return 3;
      }
      else if(strArray[0][index] == 'p' && strArray[0][index+1] == 'r' && strArray[0][index+2] == 'i' && strArray[0][index+3] == 'n' && strArray[0][index+4] == 't'){
        return 4;
      }
      return 0xff;
      break;
    case 6:
      if(strArray[0][index] == 's' && strArray[0][index+1] == 'c' && strArray[0][index+2] == 'r' && strArray[0][index+3] == 'e' && strArray[0][index+4] == 'e'&& strArray[0][index+5] == 'n'){
        return 5;
      }
      return 0xff;
      break;
    default:
      return 0xff;
      break;
  }
}
uint8_t   INTERPRETER_handleParam1(int8_t* interpreter_device, int8_t* interpreter_line){
  
  switch (firstIndex(strArray[0], ":")) {
    case 0:
      *interpreter_device = -1;
      *interpreter_line = -1;
      return 1;
      break;
    case 1:
      //TO SCREEN 0 interpreter_line 0
      return 2;
      break;
    case 2:
      //TO SCREEN interpreter_line[1] interpreter_line 0
      *interpreter_device = charToInt(strArray[0][1]);
      return 3;
      break;
      case 4:
      //TO SCREEN interpreter_line[1] interpreter_line interpreter_line[3]
      *interpreter_device = charToInt(strArray[0][1]);
      *interpreter_line = charToInt(strArray[0][3]);
      return 5;
      break;
  }
  
}

uint8_t INTERPRETER_detectOperator(char c){
  switch(c){
    case '+':
      return 0;
      break;
    case '-':
      return 1;
      break;
    case '*':
      return 2;
      break;
    case '/':
      return 3;
    default:
      return 0xff;
      break;
  }
}
void INTERPRETER_printParams(){
  int i =0;
  for(i = 0; i < ROWS; i++){
    if(strArray[i][0] != '\0'){
      printf("%s\n", strArray[i]);
    }
  }
  printf("\n");
}