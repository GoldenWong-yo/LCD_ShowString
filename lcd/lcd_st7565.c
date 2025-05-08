/*************************************************************/
// project    :	221-3059-1428
// driver IC  :	st7567
// LCD        :	1/65 duty, 1/9bias, 8.8V vop
// interface  :	SPI
// ver        :	00
// date       :  -07
// other      :	VDD 3.1v
/**************************************************************/
#include "lcd_st7565.h"
#include "lcd_func.h"
//#include "spi.h"





/* Device independent config*/
#define IO_ST7565_CS_0  GPIOD->BRR  = GPIO_PIN_4
#define IO_ST7565_CS_1  GPIOD->BSRR = GPIO_PIN_4

#define IO_ST7565_SDA_0 GPIOB->BRR  = GPIO_PIN_5
#define IO_ST7565_SDA_1 GPIOB->BSRR = GPIO_PIN_5

#define IO_ST7565_SCK_0 GPIOB->BRR  = GPIO_PIN_3
#define IO_ST7565_SCK_1 GPIOB->BSRR = GPIO_PIN_3

#define IO_ST7565_RST_0 GPIOB->BRR  = GPIO_PIN_4
#define IO_ST7565_RST_1 GPIOB->BSRR = GPIO_PIN_4

#define IO_ST7565_RS_0  GPIOD->BRR  = GPIO_PIN_7
#define IO_ST7565_RS_1  GPIOD->BSRR = GPIO_PIN_7

#define ST7565_DELAY_IO 0

static void ST7565_IO_Delay(u16 t)
{
	while(t--);
}

//rs:数据/命令标志 0,表示命令;1,表示数据;
static void ST7565_Write(u8 dat, u8 rs) //rs == 1 data rs==0  cmd
{
	u8 i;

	IO_ST7565_SCK_0;
	IO_ST7565_CS_0;
	if(!rs)
	{
		IO_ST7565_RS_0; //Command
	}else{
		IO_ST7565_RS_1;
	}
	ST7565_IO_Delay(ST7565_DELAY_IO);
	for(i=0; i<8; i++)
	{
		if( dat & 0x80)
		{
			IO_ST7565_SDA_1;
		}else{
			IO_ST7565_SDA_0;
		}
		dat <<=1;
		IO_ST7565_SCK_1;
		ST7565_IO_Delay(ST7565_DELAY_IO);
		IO_ST7565_SCK_0;
		ST7565_IO_Delay(ST7565_DELAY_IO);
	}
	// ST7565_IO_Delay(ST7565_DELAY_IO);
	IO_ST7565_CS_1;
	IO_ST7565_RS_1;
	ST7565_IO_Delay(ST7565_DELAY_IO);
}

static void ST7565_Write_Cmd(u8 cmd)
{
	ST7565_Write(cmd, 0);
}

static void ST7565_Write_Data(u8 dat)
{
	ST7565_Write(dat, 1);
}

#if 0
void ST7565_Init(void)
{
	HAL_Delay(10);
	IO_ST7565_CS_1;
	IO_ST7565_RST_0;
	HAL_Delay(10);
	IO_ST7565_RST_1;
	HAL_Delay(10);
	
	ST7565_Write_Cmd(0xAE); //Display ON
	ST7565_Write_Cmd(0xE2); //Soft Reset
	ST7565_Write_Cmd(0xA2); //Duty bias set  0xA2 is light 0xA3 is dark
	//ST7565_Write_Cmd(0xA7); //0xA7 reverse 0xA6 normal default normal
	ST7565_Write_Cmd(0xA0); //ADC select S0->S131
	ST7565_Write_Cmd(0xC8); //com1 --> com64
	ST7565_Write_Cmd(0x23); //V0 Voltage Regulator Internal Resistor Ratio Set 0x20~0x27
	HAL_Delay(2);
	ST7565_Write_Cmd(0x81); //V0 voltage set first cmd
	ST7565_Write_Cmd(0x2f & 0x3F); //following V0 voltage set 0x00~0x3F
	HAL_Delay(2);
	ST7565_Write_Cmd(0xF8); //Booster Ratio Select Mode Set first cmd
	ST7565_Write_Cmd(0x00 & 0x03); //following Booset Ratio Register Set 0x00~0x03
	HAL_Delay(2);
	ST7565_Write_Cmd(0x2F); //power control all on
	HAL_Delay(2);
	ST7565_Write_Cmd(0x10); //power control all on
	HAL_Delay(2);
	ST7565_Write_Cmd(0x40 + 0x00); //0x40 + Display Start Line(0)
	// ST7565_Fill_All(0x00); //full clear
	ST7565_Write_Cmd(0xAF); //Display ON
}
#else
/*LCD 模块初始化*/
void ST7565_Init(void)
{
	//ST7565_GPIO_Init();
	// Init_ST7565_Variables();
	// IO_ST7565_BACK_1;
	IO_ST7565_CS_0;
	IO_ST7565_RST_0;
	HAL_Delay(20);
	IO_ST7565_RST_1; /*复位完毕*/
	HAL_Delay(20);
	ST7565_Write_Cmd(0xe2); /*软复位*/
	HAL_Delay(50);
	ST7565_Write_Cmd(0x2c); /*升压步聚 1*/
	HAL_Delay(50);
	ST7565_Write_Cmd(0x2e); /*升压步聚 2*/
	HAL_Delay(50);
	ST7565_Write_Cmd(0x2f); /*升压步聚 3*/
	HAL_Delay(50);
	ST7565_Write_Cmd(0x25); /*0x24 粗调对比度，可设置范围 0x20～0x27*/
	ST7565_Write_Cmd(0x81); /*微调对比度*/
	ST7565_Write_Cmd(0x10); /*0x28,微调对比度的值，可设置范围 0x00～0x3f 1f*/
	ST7565_Write_Cmd(0xa2); /*1/9 偏压比（bias）*/
	ST7565_Write_Cmd(0xc8); /*行扫描顺序：从上到下*/
	ST7565_Write_Cmd(0xa0); /*列扫描顺序：从左到右*/
	ST7565_Write_Cmd(0x40); /*起始行：第一行开始*/
	ST7565_Write_Cmd(0xaf); /*开显示*/
	IO_ST7565_CS_1;
}
#endif

static const u8 CmdTable[] = {4,5,6,7,0,1,2,3};

//更新显存到LCD
void ST7565_Refresh_Gram(void)
{
	u8 n;

	static u8 ST7565_CurrPage = 0;
	
	//for(i=0; i<8; i++)
	{
		ST7565_Write_Cmd((0xb0 + CmdTable[ST7565_CurrPage])); //设置页地址。每页是 8 行。一个画面的 64 行被分成 8 个页。我们平常所说的第 1 页，在 LCD 驱动 IC 里是第 0 页，所以在这里减去 1*/
		ST7565_Write_Cmd((((1 >> 4) & 0x0f) + 0x10)); //设置列地址的高 4 位
		ST7565_Write_Cmd((1 & 0x0f)); //设置列地址的低 4 位
		
		for(n=0; n<128; n++)
		{
			ST7565_Write_Data(Get_Gram_Data(n, ST7565_CurrPage));
		}
		
		if(++ST7565_CurrPage >= 8)
		{
			ST7565_CurrPage = 0;
		}
	}
}

//===显示测试画面：例如全显示，隔行显示，隔列显示，雪花显示=====
void test_display(u8 data1,u8 data2)
{
	int i,j;

	for(j=0; j<8; j++)
	{
		ST7565_Write_Cmd((0xb0 + CmdTable[j])); //设置页地址。每页是 8 行。一个画面的 64 行被分成 8 个页。我们平常所说的第 1 页，在 LCD 驱动 IC 里是第 0 页，所以在这里减去 1*/
		ST7565_Write_Cmd((((1 >> 4) & 0x0f) + 0x10)); //设置列地址的高 4 位
		ST7565_Write_Cmd((1 & 0x0f)); //设置列地址的低 4 位

		for(i=0; i<128; i++)
		{
			ST7565_Write_Data(data1);
			ST7565_Write_Data(data2);
		}
	}
}
