#include "decodeUtf8.h"





int ParseUtf8String(const u8* utf8string, int* utf8code,int* unicode)
{
	u8 byte = utf8string[0];
	int numBytes, _4byteUtf8, codePoint;

	if (byte < 0x80) {
		// 1字节编码，直接返回
		numBytes = 1;
		codePoint = byte;
	} else if ((byte & 0xE0) == 0xC0) {
		// 2字节编码
		numBytes = 2;
		codePoint = byte & 0x1F;
	} else if ((byte & 0xF0) == 0xE0) {
		// 3字节编码
		numBytes = 3;
		codePoint = byte & 0x0F;
	} else if ((byte & 0xF8) == 0xF0) {
		// 4字节编码
		numBytes = 4;
		codePoint = byte & 0x07;
	} else {
		// 非法编码
		return -1;
	}

	_4byteUtf8 = byte;

	// 解析余下的字节
	for (int i = 1; i < numBytes; ++i)
	{
		byte = utf8string[i];

		if ((byte & 0xC0) != 0x80)
		{
			// 非法编码
			return -1;
		}

		_4byteUtf8 = (_4byteUtf8 << 8) | byte;
		codePoint = (codePoint << 6) | (byte & 0x3F);
	}

	*utf8code = _4byteUtf8;
	*unicode = codePoint;

	return numBytes;
}

