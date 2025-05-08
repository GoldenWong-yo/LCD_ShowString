#ifndef _LCD_CHINESE_LIB_12X12_H_
#define _LCD_CHINESE_LIB_12X12_H_

#include "global.h"





#define MAX_CHINESE_WORD_NUM 200

typedef struct {
	int utf8code;
	u8 dchar[40];
}ST_CHINESE;

extern const ST_CHINESE ChineseLib_12x12[MAX_CHINESE_WORD_NUM];

#endif
