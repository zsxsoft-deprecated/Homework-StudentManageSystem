#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
int gbk_to_utf8(unsigned char *gbk_str, unsigned char *utf8_str, int utf8_len);
time_t get_unix_timestamp(int y, int m, int d);
