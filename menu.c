#include "stdafx.h"
#define MENU_MAX_SIZE 10
typedef struct menu_data {
	char input_key;
	char *input_string;
	void (*function_pointer)(void); 
} menu_data;
menu_data menu[MENU_MAX_SIZE];
menu_data submenu[];// [MENU_MAX_SIZE];
int is_menu_initialized = 0;
int menu_current_size = -1;

void append_menu_item(char input_key, char* input_string, void (*function_pointer)(void)) {
	menu_current_size++;
	menu_data *temp = (struct menu_data *)malloc(sizeof(struct menu_data));
	temp->input_key = input_key;
	temp->input_string = input_string;
	temp->function_pointer = function_pointer;
	menu[menu_current_size] = *temp;
}

void get_time (char* output) {
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	sprintf(output, "现在是： %d 年 %d 月 %d 日（星期%d） %d:%d:%d\n",
		timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_wday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void print_welcome(void) {
	CHAR_WRAPPER(time, 100,
		get_time(time);
		printf("%s", time);
	)
}

void print_menu(void) {
	int i;
	printf("****************************************\n");
	for (i = 0; i <= menu_current_size; i++) {
		printf("*   %c:  %-31s*\n", menu[i].input_key, menu[i].input_string);
	}
	printf("****************************************\n");
}

void do_menu(void) {
	char input_key;
	int i;
	while (1) {
		print_menu();
		printf("请选择：( ");
		for (i = 0; i <= menu_current_size; i++) {
			printf("%c ", menu[i].input_key);
		}
		printf(")：");
		int flag = 1;
		scanf("%c", &input_key);
		if (input_key == '\n' || input_key == '\r' || input_key == ' ') continue;
		if (input_key > 'a') input_key -= ('a' - 'A'); // 小写识别
		for (i = 0; i <= menu_current_size; i++) {
			if (menu[i].input_key == input_key) {
				flag = 0;
				menu[i].function_pointer();
			}
		}
		if (flag == 1) {
			printf("格式错误，请重新输入！\n");
		}
		eat_line();
	}
}
