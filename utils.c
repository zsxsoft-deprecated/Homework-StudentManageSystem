#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

time_t get_unix_timestamp(int y, int m, int d) {
	struct tm tm;
	time_t timestamp;
	tm.tm_year = y - 1900;
	tm.tm_mon = m - 1;
	tm.tm_mday = d;
	tm.tm_hour = 0;
	tm.tm_min = 0;
	tm.tm_sec = 0;
	timestamp = mktime(&tm);
	return timestamp;
}

int utf8_to_gbk(unsigned char *utf8_str, unsigned char *gbk_str)
{
	wchar_t *unicode_str;
	int ret;
	if (!utf8_str) return 0;
	ret = MultiByteToWideChar(CP_UTF8, 0, (char *)utf8_str, -1, NULL, (int)NULL);
	unicode_str = malloc(sizeof(wchar_t) * (ret + 1));
	ret = MultiByteToWideChar(CP_UTF8, 0, (char *)utf8_str, -1, unicode_str, ret);
	if (!ret) return 0;
	ret = WideCharToMultiByte(CP_ACP, 0, unicode_str, -1, NULL, (int)NULL, NULL, NULL);
	if (!gbk_str) {
		if (unicode_str) free(unicode_str);
		return ret;
	}
	ret = WideCharToMultiByte(CP_ACP, 0, unicode_str, -1, (char *)gbk_str, ret, NULL, NULL); 

	if (unicode_str) free(unicode_str);

	return ret;
}

int gbk_to_utf8(unsigned char *gbk_str, unsigned char *utf8_str, int utf8_len) {
	wchar_t *unicode_str;
	int ret;
	if (!gbk_str) return 0;
	ret = MultiByteToWideChar(CP_ACP, 0, (char *)gbk_str, -1, NULL, (int)NULL);
	unicode_str = malloc(sizeof(wchar_t) * (ret + 1));
	ret = MultiByteToWideChar(CP_ACP, 0, (char *)gbk_str, -1, unicode_str, ret);
	if (!ret) return 0;
	ret = WideCharToMultiByte(CP_UTF8, 0, unicode_str, -1, NULL, 0, NULL, NULL);
	if (!utf8_str) {
		if (unicode_str) free(unicode_str);
		return ret;
	}
	ret = WideCharToMultiByte(CP_UTF8, 0, unicode_str, -1, (char *)utf8_str, utf8_len, NULL, NULL);

	if (unicode_str) free(unicode_str);

	return ret;
}


int utf8_to_gbk_all(unsigned char *utf8_str, unsigned char **gbk_str)
{
	int gbk_len = utf8_to_gbk((unsigned char *)utf8_str, NULL);
	*gbk_str = malloc(sizeof(char) * (gbk_len + 1));
	return utf8_to_gbk((unsigned char *)utf8_str, (unsigned char *)*gbk_str);
}

int gbk_to_utf8_all(unsigned char *gbk_str, unsigned char **utf8_str)
{
	int utf8_len = gbk_to_utf8((unsigned char *)gbk_str, NULL, (int)NULL);
	*utf8_str = malloc(sizeof(char) * (utf8_len + 1));
	return gbk_to_utf8((unsigned char *)gbk_str, (unsigned char *)*utf8_str, utf8_len);
}