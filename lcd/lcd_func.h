#ifndef _LCD_FUNC_H_
#define _LCD_FUNC_H_

#include "global.h"





typedef struct{
	u16 X;
	u16 Y;
}ST_LCD_CURSOR;

void LCD_Clear(void);
u8 Get_Gram_Data(u16 X, u16 Page);
ST_LCD_CURSOR LCD_GetCursor(void);
void LCD_Switch_Next_Line(void);

typedef enum{
	Lcd_Show_Normal = 0,
	Lcd_Show_Opposite,
}EN_Lcd_Show_Mode;

void LCD_AppendString(const u8* txt, EN_Lcd_Show_Mode ShowMode);
void LCD_ShowString(u16 X, u16 Y, const u8* txt, EN_Lcd_Show_Mode ShowMode);

#endif
