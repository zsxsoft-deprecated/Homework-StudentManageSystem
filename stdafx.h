#pragma once
#ifdef _MSC_VER
#include "targetver.h"
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif 

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <time.h>
#include <float.h>

#define GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define MEMORY_WRAPPER(type, output, size, fn) \
	type* output = (type*)malloc(sizeof(type) * size); \
	fn \
	free(output); \
	output = NULL; 

#define CHAR_WRAPPER(output, size, fn) MEMORY_WRAPPER(char, output, size, fn)
#define WCHAR_WRAPPER(output, size, fn) MEMORY_WRAPPER(wchar_t, output, size, fn)
void eat_line(void);