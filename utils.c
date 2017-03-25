#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

time_t get_unix_timestamp(int y, int m, int d) {
	struct tm tm;
	time_t timestamp = 0;
	tm.tm_year = y - 1900;
	tm.tm_mon = m - 1;
	tm.tm_mday = d;
	tm.tm_hour = 0;
	tm.tm_min = 0;
	tm.tm_sec = 0;
	timestamp = mktime(&tm);
	return timestamp;
}


int gbk_to_utf8(unsigned char *gbk_str, unsigned char *utf8_str, int utf8_len) {
	wchar_t * unicode_str = NULL;
	int ret = 0;
	if (!gbk_str) return 0;
	ret = MultiByteToWideChar(CP_ACP, 0, (char *)gbk_str, -1, NULL, (int)NULL);
	unicode_str = malloc(sizeof(wchar_t) * (ret + 1));
	ret = MultiByteToWideChar(CP_ACP, 0, (char *)gbk_str, -1, unicode_str, ret);
	if (!ret) return 0;
	ret = WideCharToMultiByte(CP_UTF8, 0, unicode_str, -1, NULL, 0, NULL, NULL); 
	if (!utf8_str) {
		if (unicode_str)
			free(unicode_str);
		return ret;
	}
	ret = WideCharToMultiByte(CP_UTF8, 0, unicode_str, -1, (char *)utf8_str, utf8_len, NULL, NULL);  //×ª»»µ½UTF8±àÂë

	if (unicode_str)
		free(unicode_str);

	return ret;
}
