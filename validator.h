#pragma once
int input_int(int *input, int validator(int *data));
int input_char(char *input, int validator(char *data));
int input_string(char *input, int validator(char *data));
int input_wchar(wchar_t *input, int validator(wchar_t *data));

int name_validator(char *data);
int sex_validator(char **data);
int amount_validator(double *data);