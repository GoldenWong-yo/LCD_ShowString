1.主要功能：采用utf-8编码，一个函数一次性显示一行字符，支持中文英文共显。
2.特点：轻量化，目前只支持显示文字类信息，并且只适用于中文字库小的项目。已添加完成ascii字库。不需要完整中文字库，用哪些添加哪些。
3.缺点：显示中文需要遍历对比字符id，效率低。
4.注意：在小端平台上，需要注意在 ParseUtf8String() 函数中传入的 utf8string 参数的，需要检查是否需要作字节序调换。

1. Main functions: utf-8 encoding is adopted. One function displays one line of characters at a time and supports simultaneous display of Chinese and English.
2. Features: Lightweight, currently only supports the display of text-based information, and is only suitable for projects with small Chinese font libraries. The ascii character library has been added. There is no need for a complete Chinese font library. Add whatever is used.
3. Disadvantages: Displaying Chinese characters requires traversing and comparing character ids, which is inefficient.
4. Note: On a little-endian platform, it is necessary to pay attention to the utf8string parameter passed in the ParseUtf8String() function and check whether byte order swapping is required.
