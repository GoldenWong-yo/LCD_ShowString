#include "lcd_func.h"
#include "lcd_charlib_6x12.h"
#include "lcd_chineselib_12x12.h"
#include "decodeUtf8.h"





#define	LCD_X_PIXEL 128	//横向宽度
#define	LCD_Y_PIXEL 64	//纵向高度

//一个数据的大小（单位数据大小）
#define LCD_DATA_SIZE 8

#define LCD_PAGE_NUM (LCD_Y_PIXEL / LCD_DATA_SIZE)

typedef struct{
	u8 Gram[LCD_X_PIXEL][LCD_PAGE_NUM];
	ST_LCD_CURSOR Cursor;
	// u8 CurrPage;
}ST_LCD;

ST_LCD whyLCD;

void LCD_Clear(void)
{
	memset(&whyLCD, 0, sizeof(whyLCD));
}

u8 Get_Gram_Data(u16 X, u16 Page)
{
	return whyLCD.Gram[X][Page];
}

ST_LCD_CURSOR LCD_GetCursor(void)
{
	return whyLCD.Cursor;
}

static void LCD_SetCursor(u16 X, u16 Y)
{
	whyLCD.Cursor.X = X;
	whyLCD.Cursor.Y = Y;
}

void LCD_Switch_Next_Line(void)
{
	whyLCD.Cursor.X  = 0;
	whyLCD.Cursor.Y += 16;
}

static void LCD_DrawPoint(u16 X, u16 Y, bool DataPoint)
{
	u16 Page, bitY, temp = 0;

	if(X >= LCD_X_PIXEL || Y >= LCD_Y_PIXEL)
	{
		return;				//超出范围了.
	}

	Page = Y / 8;
	bitY = Y % 8;
	temp = 1 << bitY;

	if(DataPoint)
	{
		whyLCD.Gram[X][Page] |= temp;
	}else{
		whyLCD.Gram[X][Page] &= (~temp);
	}
}

static void LCD_ShowPoint(u16 X, u16 Y, bool DataPoint, EN_Lcd_Show_Mode ShowMode)
{
	if(ShowMode == Lcd_Show_Normal)
	{
		if(DataPoint)
		{
			LCD_DrawPoint(X, Y, 1);
		}else{
			LCD_DrawPoint(X, Y, 0);
		}
	}else{
		if(DataPoint)
		{
			LCD_DrawPoint(X, Y, 0);
		}else{
			LCD_DrawPoint(X, Y, 1);
		}
	}
}

static void LCD_ShowChar(const u8* pData, u16 X_Size, u16 Y_Size, EN_Lcd_Show_Mode ShowMode)
{
	u16 NumPage = 0;
	u16 Remain = 0;
	u16 Page = 0;
	u16 X = 0;
	u16 TempX = 0;
	u16 TempY = 0;
	u16 TempEndY = 0;
	u8 TempData = 0;

	NumPage = Y_Size / 8;
	Remain = Y_Size % 8;

	if(Remain)
	{
		NumPage += 1;
	}

	if(NumPage < 1)
	{
		return;
	}

	for(Page=0; Page<NumPage; Page++)
	{
		for(X=0; X<X_Size; X++)
		{
			TempData = pData[Page * X_Size + X];

			TempX = whyLCD.Cursor.X + X;
			TempY = whyLCD.Cursor.Y + Page * 8;

			if((Remain != 0) && (Page + 1 == NumPage))
			{
				TempEndY = TempY + Remain;
			}else{
				TempEndY = TempY + 8;
			}

			for(; TempY<TempEndY; TempY++)
			{
				LCD_ShowPoint(TempX, TempY, (TempData & 0x01), ShowMode);

				TempData >>= 1;
			}
		}
	}

	whyLCD.Cursor.X += X_Size;
}

static u16 Chinese_TXT_Order(int utf8code)
{
	u16 i;

	for(i=0; i<MAX_CHINESE_WORD_NUM; i++)
	{
		//如果 utf8code 为 0，表示已经查找到了字符表的末尾还没有找到
		if(ChineseLib_12x12[i].utf8code == 0)
		{
			break;
		}
		else if(ChineseLib_12x12[i].utf8code == utf8code)
		{
			return i;
		}
	}

	return 0;
}

void LCD_AppendString(const u8* txt, EN_Lcd_Show_Mode ShowMode)
{
	u16 i = 0;
	int numBytes = 0;
	int utf8code = {0};
	int unicode = 0;
	u16 txtOrder = 0;
	const u8* pData;

	while(txt[i] != 0)
	{
		numBytes = ParseUtf8String(txt[i], &utf8code, &unicode);
		if(numBytes == -1)
		{
			_DEBUG("Error: Invalid UTF-8 sequence\n");

			i += 1;

			continue;
		}else{
			i += numBytes;
		}

		//如果是 ascii 码
		if(numBytes == 1)
		{
			if(utf8code < ' ')
			{
				return;
			}else{
				txtOrder = utf8code - ' ';
			}

			pData = CharLib_6x12[txtOrder];

			LCD_ShowChar(pData, 6, 12, ShowMode);
		}else{
			txtOrder = Chinese_TXT_Order(utf8code);

			pData = ChineseLib_12x12[txtOrder].dchar;

			LCD_ShowChar(pData, 12, 12, ShowMode);
		}
	}
}

void LCD_ShowString(u16 X, u16 Y, const u8* txt, EN_Lcd_Show_Mode ShowMode)
{
	LCD_SetCursor(X, Y);

	LCD_AppendString(txt, ShowMode);
}
