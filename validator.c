#include "stdafx.h"
#include "validator.h"

#define INPUT_DATA_WITHOUT_SCANF(read_function, name, format, type) \
	int name(type *input, int validator(type *data)) { \
		read_function(format, input); \
		if (validator != NULL) { \
			return validator(input); \
		} \
		return 0; \
	}

#define INPUT_DATA(name, format, type) INPUT_DATA_WITHOUT_SCANF(scanf, name, format, type)
#define INPUT_DATA_WIDE(name, format, type) INPUT_DATA_WITHOUT_SCANF(scanf, name, format, type)

INPUT_DATA(input_int, "%d", int)
INPUT_DATA(input_char, "%c", char)
INPUT_DATA(input_string, "%s", char)
INPUT_DATA_WIDE(input_wchar, "%ls", wchar_t)

int name_validator(char *data) {
	int len = (int)strlen(data);
	if (len < 2 * 2 || len > 4 * 2) return 1;
	return 0;
}

int sex_validator(char **data) {
	if (strcmp("M", *data) == 0 || strcmp("F", *data) == 0) {
		*data = !strcmp("M", *data) ? "ÄÐ" : "Å®";
		return 0;
	}
	if (strcmp("ÄÐ", *data) != 0 && strcmp("Å®", *data) != 0) return 1;
	return 0;
}

int amount_validator(double *data) {
	return !(*data >= 0 && *data <= 9999);
}

#undef INPUT_DATA_WITHOUT_SCANF
#undef INPUT_DATA
#undef INPUT_DATA_WIDE