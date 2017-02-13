#pragma once
void append_menu_item(char input_key, char* input_string, void(*function_pointer)(void));
void get_time (char* output);
void print_welcome(void);
void print_menu(void);
char do_menu(void);