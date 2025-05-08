#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_

#include "global.h"
#include "stm32f1xx.h"





typedef enum{
	LCD_TEST_NULL = 0,
	LCD_TEST_MAX,
}EN_LCD_SHOW;


void Display_Init(void);
void Set_Display(EN_LCD_SHOW Show);
void Display_Handle(void);


#endif


