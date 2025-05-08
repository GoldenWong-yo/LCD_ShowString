#include "lcd_display.h"
#include "lcd_st7565.h"
#include "lcd_func.h"
#include "gpio.h"





typedef struct{
	EN_LCD_SHOW PrevShow;
	EN_LCD_SHOW CurrShow;
}ST_LCD_SHOW;

static ST_LCD_SHOW LcdShow;

void Display_Init(void)
{
	LcdShow.PrevShow = LCD_TEST_MAX;
	LcdShow.CurrShow = LCD_TEST_NULL;

	ST7565_Init();
}

void Set_Display(EN_LCD_SHOW Show)
{
	LcdShow.CurrShow = Show;
}

void Display_Handle(void)
{
	u8 txt[100] = {0};
	float test = 0.123456789f;

	if(LcdShow.PrevShow != LcdShow.CurrShow)
	{
		LcdShow.PrevShow = LcdShow.CurrShow;

		LCD_Clear();

		LCD_SetCursor(34,0);

		LCD_ShowString("test测试", Lcd_Show_Normal);

		LCD_SetCursor(0,32);

		switch(LcdShow.CurrShow)
		{
			case LCD_TEST_NULL:
				sprintf(txt, "123test测试中: %03f ", test);
				LCD_ShowString(txt, Lcd_Show_Normal);
				break;
			default:
				break;
		}
	}
}



